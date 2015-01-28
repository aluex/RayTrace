//
//  main.cpp
//  RayTracer
//
//  Created by Aluex on 1/18/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#include <cstdio>
#include <opencv2/opencv.hpp>
//#include <iostream>
#include <string>
#include "Frame.h"
#include "Sphere.h"
#include "Render.h"
#include "Plane.h"
#include "Defaults.h"
#include "Sampler.h"
#include "Triangle.h"
#include "Box.h"
#include "OctoTree.h"
#include "Body.h"
#include "picojson.h"

#define PI (3.14159)
int weight = 640, height = 480;
char const *outputFile = "output.jpg";
void showHelp()
{
    printf("raytracer outputFile [scene.json]\n");
}

void doRayTrace()
{
    cv::Mat M(height, weight, CV_8UC3, cv::Scalar::all(0));
#ifdef RAY_DEBUG
    //std::cout << Vector(1,2,3) * Vector(3,4,5) << std::endl;
#endif
    
    ////////// Build Scene //////////
    //// Sampler
    Sampler *sampler = new JitterSampler(DEFAULT_HN * DEFAULT_WN, 5);
    //Sampler *sampler = new PureRandomSampler(DEFAULT_HN * DEFAULT_WN, 5);
    sampler->generateSamples();
    sampler->setupShuffledIndices();
    //// Frame
    double cameraTheta = 0, cameraRadius = 100.;
    Frame frame(height, weight, 0.05);
    frame.sampler = sampler;
    frame.center = Point3D(0,0,20);
    frame.rightHandSide = -1;
    frame.e1 = Vector(sin(cameraTheta), 0, cos(cameraTheta)); //Row vector
    frame.e2 = Vector(0, 1, 0); //Column vector
    //// Camera
    Camera camera;
    camera.position = frame.center + Point3D(-cameraRadius, 0, cameraRadius * tan(cameraTheta));
    camera.normal = Vector(1,0,-1);
    camera.aperture = 5;
    camera.focalDis = 12;
    frame.camera = &camera;
    //// Lights
    Light *l1 = new Light(Point3D(0,0,40), RGBColor(255,255,255), 1, 4),
        *l2 = new Light(Point3D(6,7,40), RGBColor(255,255,255), 1, 4),
    *l3 = new Light(Point3D(12, 7, 46), RGBColor(255,255,255), 1, 4);
    //// Materials
    BasicMaterial sphereMaterial;
    sphereMaterial.reflectionFact = 0.6;
    BasicMaterial planeMaterial;
    planeMaterial.reflectionFact = 0.1;
    planeMaterial.refractionFact = 0;
    planeMaterial.diffuseFact = Vector(1,1,1);
    planeMaterial.glossyFact = 0.5;
    BasicMaterial imgMaterial;
    imgMaterial.specularFact = Vector(0.2,0.2,0.2);
    imgMaterial.diffuseFact = Vector(1,1,1);
    imgMaterial.reflectionFact = 0;
    imgMaterial.refractionFact = 0;
    BasicMaterial glassMaterial;
    glassMaterial.reflectionFact = 0.10;
    glassMaterial.refractionFact = 0.90;
    glassMaterial.specularPower = 100;
    glassMaterial.diffuseFact = Vector(0.1,0.1,0);
    glassMaterial.N = 1.2; // Glass
    BasicMaterial objMaterial;
    objMaterial.reflectionFact = 0.5;
    objMaterial.refractionFact = 0.0;
    objMaterial.diffuseFact = Vector(1,1,1);
    objMaterial.specularPower = 30;
    //// Objects
    Geometry *s1 = new Sphere(Point3D(80, 0, 14), 4),
        *s2 = new Sphere(Point3D(80, 8, 16), 4, &sphereMaterial),
    *glassSphere = new Sphere(Point3D(4,3.5, 4), 3, &glassMaterial),
        *p = new Plane(Point3D(1, 0, 0), Vector(0, 0, 1), &planeMaterial),
    //*p = new GridPlane(Point3D(1, 0, 0), Vector(0, 0, 1), &planeMaterial, 40.0),
    //*p2 = new ImagePlane(Point3D(100,0,-1), Vector(-1,0,0), &imgMaterial, 0.04, cv::imread("background.jpg",1),
    //  Vector(0,0,-1), Vector(0,-1,0), 600, 800),
    *p2 = new ImagePlane(Point3D(300, 0, -1), Vector(-1,0,0), &imgMaterial, 0.1, cv::Mat(cvLoadImage("background.jpg",1)),
        Vector(0,0,-1), Vector(0,-1,0), 600, 1000, true), // yBias, xBias
    *t1 = new Triangle(Vector(4, -2, 10), Vector(2, -4, 4), Vector(2,0,4), &glassMaterial),
    *t2 = new Triangle(Vector(4, -2, 10), Vector(2, 0, 4), Vector(6,0,4), &glassMaterial),
    *t3 = new Triangle(Vector(4, -2, 10), Vector(6, 0, 4), Vector(6,-4,4), &glassMaterial),
    *t4 = new Triangle(Vector(4, -2, 10), Vector(6, -4, 4), Vector(2,-4,4), &glassMaterial),
    *t5 = new Triangle(Vector(4, -2, -2), Vector(2, -4, 4), Vector(2,0,4), &glassMaterial),
    *t6 = new Triangle(Vector(4, -2, -2), Vector(2, 0, 4), Vector(6,0,4), &glassMaterial),
    *t7 = new Triangle(Vector(4, -2, -2), Vector(6, 0, 4), Vector(6,-4,4), &glassMaterial),
    *t8 = new Triangle(Vector(4, -2, -2), Vector(6, -4, 4), Vector(2,-4,4), &glassMaterial),
    *b = new Box(Vector(4,-2,-6), Vector(7,9,-4), &planeMaterial)
    ;
    char const * fileName = "patterngenerator_script_example1.obj";
    std::cout << "Loading obj..." << std::endl;
    //Body body(fileName, Vector(100, 0, -30), 7, Vector(1,0,-1), 0, Vector(255,255,255)*0.8, &objMaterial);
    Body body(fileName, Vector(120, 0, 12), 0.2, Vector(0,0,1), -3.14159/4, Vector(255,255,255)*0.2, &objMaterial);
    std::cout << "Loaded." << std::endl;
    OctoTree oTree(body.triList);
    Geometry *ot = &oTree;
    s1->setColor(RGBColor(255, 0, 0));
    s2->setColor(RGBColor(0, 0, 255));
    glassSphere->setColor(RGBColor(255,255,255));
    p->setColor(RGBColor(255, 255, 255));
    b->setColor(RGBColor(255, 255, 255));
    //oTree.setColor(RGBColor(255,255,255) * 0.8);
    //// Set-up the scene
    Scene scene;
    //scene.addObject(s1);
    //scene.addObject(s2);
    //scene.addObject(glassSphere);
    
    scene.addObject(p);
    scene.addObject(p2);
    //scene.addObject(t1);
    //scene.addObject(t2);
    //scene.addObject(t3);
    //scene.addObject(t4);
    //scene.addObject(t5);
    //scene.addObject(t6);
    //scene.addObject(t7);
    //scene.addObject(t8);
    //scene.addObject(ot);
    //scene.addObject(b);
    scene.addLight(l1);
    scene.addLight(l2);
    scene.addLight(l3);
    
    //// Set-up the render
    Render render(M, height, weight, DEFAULT_HN, DEFAULT_WN, DEFAULT_INV_GAMMA, 4, 0.3, 0);
    render.frame = &frame;
    render.scene = &scene;
    
    render.doRender();
    ////////// Output the image ugly because of problems of incompatibility concerning libstdc++ and opencv
    IplImage *img_Output = cvCreateImage(cvSize(M.cols,M.rows),8,3);
    IplImage ipltemp=M;
    cvCopy(&ipltemp, img_Output);
    cvSaveImage(outputFile, img_Output);
    cvShowImage("RayTrace", img_Output);
    //cv::imwrite(outputFile, M);
    //cv::imshow("RayTrace", M);
    getchar();
}
#define MAX_JSON_FILE_COUNT 5000001
char jsonBuffer[MAX_JSON_FILE_COUNT];
Vector parseVector(picojson::array & vecJson)
{
    return Vector(vecJson[0].get<double>(),
                  vecJson[1].get<double>(),
                  vecJson[2].get<double>());
}
void doRayTraceBasedOnJson(const char *jsonFileName)
{
    FILE *jsonF = fopen(jsonFileName, "r");
    size_t numRead = fread(jsonBuffer, sizeof(char), sizeof(jsonBuffer), jsonF);
    fclose(jsonF);
    picojson::value v;
    std::string err;
    char const *constPtr = jsonBuffer;
    int MAX_DEPTH = 4; double secondRayControl = 0.3; int extraRayTest = 0;
    picojson::parse(v, constPtr, constPtr + numRead, &err);
    if(! err.empty()) std::cerr << err << std::endl;
    if(!v.is<picojson::object>())
    {
        std::cerr << "Bad json." << std::endl;
    }
    Scene scene;
    std::vector<Sampler*> samplers;
    std::vector<Material*> materials;
    //std::vector<Geometry*> geometries;
    //std::vector<Light*> lights;
    //////////////////////////
    picojson::object & obj = v.get<picojson::object>();
    //////////////////////////
    ///// Read Default ///////
    if(obj.find("DEFAULT_EPSILON")!=obj.end())
        DEFAULT_EPSILON = obj["DEFAULT_EPSILON"].get<double>();
    if(obj.find("DEFAULT_DOTSIZE")!=obj.end())
        DEFAULT_DOTSIZE = obj["DEFAULT_DOTSIZE"].get<double>();
    if(obj.find("DEFAULT_INV_GAMMA")!=obj.end())
        DEFAULT_INV_GAMMA = obj["DEFAULT_INV_GAMMA"].get<double>();
    if(obj.find("DEFAULT_PERSPECTIVE")!=obj.end())
        DEFAULT_PERSPECTIVE = obj["DEFAULT_PERSPECTIVE"].get<bool>();
    if(obj.find("DEFAULT_HN")!=obj.end())
        DEFAULT_HN = int(obj["DEFAULT_HN"].get<double>());
    if(obj.find("DEFAULT_WN")!=obj.end())
        DEFAULT_WN = int(obj["DEFAULT_WN"].get<double>());
    if(obj.find("MAX_DEPTH")!=obj.end())
        MAX_DEPTH = int(obj["MAX_DEPTH"].get<double>());
    if(obj.find("secondRayControl")!=obj.end())
        secondRayControl = obj["secondRayControl"].get<double>();
    if(obj.find("extraRayTest")!=obj.end())
        extraRayTest = int(obj["extraRayTest"].get<double>());
    //////////////////////////
    picojson::array samplerArray = obj["Samplers"].get<picojson::array>();
    for(picojson::array::iterator it = samplerArray.begin(); it!=samplerArray.end() ;++it)
    {
        picojson::object & samplerObj = (*it).get<picojson::object>();
        std::string samplerType = samplerObj["type"].get<std::string>();
        int samplerCapacity = int(samplerObj["capacity"].get<double>());
        int samplerSetNum = int(samplerObj["numSet"].get<double>());
        Sampler *s=NULL;
        if(samplerType=="JitterSamper")
        {
            s = new JitterSampler(samplerCapacity, samplerSetNum);
        }
        else if(samplerType=="PureRandomSampler")
        {
            s = new PureRandomSampler(samplerCapacity, samplerSetNum);
        }
        s->generateSamples();
        s->setupShuffledIndices();
        samplers.push_back(s);
    }
    picojson::object cameraObj = obj["Camera"].get<picojson::object>();
    Camera camera;
    double cameraTheta = cameraObj["cameraTheta"].get<double>();
    double cameraRadius = cameraObj["cameraRadius"].get<double>();
    camera.aperture = cameraObj["aperture"].get<double>();
    camera.focalDis = cameraObj["focalDis"].get<double>();
    camera.normal = parseVector(cameraObj["normal"].get<picojson::array>());
    picojson::object frameObj = obj["Frame"].get<picojson::object>();
    int height = int(frameObj["height"].get<double>()), width = int(frameObj["width"].get<double>()), samplerIndex = int(frameObj["samplerIndex"].get<double>());
    double dotSize = frameObj["dotSize"].get<double>();
    Frame frame(height, width, dotSize);
    frame.sampler = samplers[samplerIndex];
    frame.center = parseVector(frameObj["center"].get<picojson::array>());
    frame.rightHandSide = int(frameObj["rightHandSide"].get<double>());
    frame.e1 = Vector(sin(cameraTheta), 0, cos(cameraTheta));
    frame.e2 = Vector(0, 1, 0);
    camera.position = frame.center + Point3D(-cameraRadius, 0, cameraRadius * tan(cameraTheta));
    frame.camera = &camera;
    picojson::array lightArr = obj["Lights"].get<picojson::array>();
    for(picojson::array::iterator it = lightArr.begin(); it!=lightArr.end(); ++it)
    {
        picojson::object & lightObj = (*it).get<picojson::object>();
        scene.addLight(new Light(
            parseVector(lightObj["position"].get<picojson::array>()),
            parseVector(lightObj["color"].get<picojson::array>()),
            lightObj["intensity"].get<double>(), lightObj["size"].get<double>()
        ));
    }
    picojson::array materialArr = obj["Materials"].get<picojson::array>();
    for(picojson::array::iterator it = materialArr.begin(); it!=materialArr.end(); ++it)
    {
        Material *matTmp = new Material;
        picojson::object & matObj = (*it).get<picojson::object>();
        if(matObj.find("reflectionFact")!=matObj.end())
            matTmp->reflectionFact = matObj["reflectionFact"].get<double>();
        if(matObj.find("refractionFact")!=matObj.end())
            matTmp->refractionFact = matObj["refractionFact"].get<double>();
        if(matObj.find("diffuseFact")!=matObj.end())
            matTmp->diffuseFact = parseVector(matObj["diffuseFact"].get<picojson::array>());
        if(matObj.find("specularFact")!=matObj.end())
            matTmp->specularFact = parseVector(matObj["specularFact"].get<picojson::array>());
        if(matObj.find("glossyFact")!=matObj.end())
            matTmp->glossyFact = matObj["glossyFact"].get<double>();
        if(matObj.find("specularPower")!=matObj.end())
            matTmp->specularPower = matObj["specularPower"].get<double>();
        if(matObj.find("N")!=matObj.end())
            matTmp->N = matObj["N"].get<double>();
        materials.push_back(matTmp);
    }
    picojson::array objArr = obj["Objects"].get<picojson::array>();
    for(picojson::array::iterator it = objArr.begin(); it!=objArr.end(); ++it)
    {
        picojson::object &geoObj = (*it).get<picojson::object>();
        if(geoObj.find("enabled")==geoObj.end())
            continue;
        bool enabled = geoObj["enabled"].get<bool>();
        if(!enabled) continue;
        std::string geoType = geoObj["geometry"].get<std::string>();
        Geometry *g;
        if(geoType=="Sphere")
        {
            
            g = new Sphere(parseVector(geoObj["position"].get<picojson::array>()), geoObj["radius"].get<double>());
        }
        else if(geoType=="Plane")
        {
            g = new Plane(parseVector(geoObj["position"].get<picojson::array>()),parseVector(geoObj["normal"].get<picojson::array>()));
        }
        else if(geoType=="GridPlane")
        {
            g = new GridPlane(parseVector(geoObj["position"].get<picojson::array>()),parseVector(geoObj["normal"].get<picojson::array>()), geoObj["gridSize"].get<double>());
        }
        else if(geoType=="ImagePlane")
        {
            g = new ImagePlane(
                               parseVector(geoObj["position"].get<picojson::array>()),
                               parseVector(geoObj["normal"].get<picojson::array>()),
                               geoObj["gridSize"].get<double>(),
                               cv::Mat(cvLoadImage(geoObj["picSource"].get<std::string>().c_str(),1)),
                               parseVector(geoObj["e1"].get<picojson::array>()), parseVector(geoObj["e2"].get<picojson::array>()), int(geoObj["heightBias"].get<double>()), int(geoObj["widthBias"].get<double>()), geoObj["repeat"].get<bool>());
        }
        else if(geoType=="Triangle")
        {
            picojson::array & pointList = geoObj["points"].get<picojson::array>();
            g = new Triangle(
                parseVector(pointList[0].get<picojson::array>()),
                             parseVector(pointList[0].get<picojson::array>()),
                             parseVector(pointList[0].get<picojson::array>())
            );
        }
        else if(geoType=="Box")
        {
            picojson::array & pointList = geoObj["points"].get<picojson::array>();
            g = new Box(parseVector(pointList[0].get<picojson::array>()),parseVector(pointList[0].get<picojson::array>()));
        }
        else if(geoType=="Model")
        {
            std::string fileName = geoObj["fileName"].get<std::string>();
            Vector bias = parseVector(geoObj["bias"].get<picojson::array>()), rotateAxis = parseVector(geoObj["rotateAxis"].get<picojson::array>());
            double scale = geoObj["scale"].get<double>(), rotateDegree = geoObj["rotateDegree"].get<double>();
            Body body(fileName.c_str(), bias, scale, rotateAxis, rotateDegree, parseVector(geoObj["color"].get<picojson::array>()), materials[int(geoObj["materialIndex"].get<double>())]);
            g = new OctoTree(body.triList);
        }
        else continue; // Not recognizable
        if(geoObj.find("materialIndex")!=geoObj.end())
            g->material = materials[int(geoObj["materialIndex"].get<double>())];
        else g->material = materials[0];
        if(geoObj.find("color")!=geoObj.end())
        g->color = parseVector(geoObj["color"].get<picojson::array>());
        else g->color = Vector(255,255,255);
        scene.addObject(g);
    }
    //////////////////////////
    cv::Mat M(frame.height, frame.width, CV_8UC3, cv::Scalar::all(0));
    //Render render(M, frame.height, frame.width);
    Render render(M, frame.height, frame.width, DEFAULT_HN, DEFAULT_WN, DEFAULT_INV_GAMMA, MAX_DEPTH, secondRayControl, extraRayTest);
    render.frame = &frame;
    render.scene = &scene;
    render.doRender();
    IplImage *img_Output = cvCreateImage(cvSize(M.cols,M.rows),8,3);
    IplImage ipltemp=M;
    cvCopy(&ipltemp, img_Output);
    cvSaveImage(outputFile, img_Output);
    cvShowImage("RayTrace", img_Output);
    //cv::imwrite(outputFile, M);
    //cv::imshow("RayTrace", M);
    getchar();
    //////////////////////////
}
int main(int argc, const char * argv[]) {
    //doRayTraceBasedOnJson("scene.json");
    //return 0;
    if(argc>1)
    {
        if (argc>1) {
            outputFile = (char*)argv[1];
        }
        if(argc >2)
        {
            doRayTraceBasedOnJson(argv[2]);
        }
        else doRayTrace();
    }
    else showHelp();
    return 0;
}
