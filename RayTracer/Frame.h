//
//  Frame.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Frame__
#define __RayTracer__Frame__

#include <stdio.h>
#include <cmath>
#include "Camera.h"
#include "Prefix.h"
#include "Ray.h"
#include "Defaults.h"
#include "Sampler.h"

class Frame
{
public:
    int height, width; // Number of pixels
    Point3D center;
    Vector e1, e2; // Assuming e1 and e2 are normalized, e1 x e2 is the z
    //Vector normal;
    double rotate;
    double dotSize;
    int rightHandSide;
    Camera *camera;
    Sampler *sampler;
    bool perspective; // whether or not to assume the camera is infinitely far away from the view frame
    static int hn, wn;
    void setSampler(Sampler *sp)
    {
        if(sampler) delete sampler;
        sampler = sp;
    }
    bool getDirectionWithSamplerWithDOF(int i, int j, Ray & ray)
    {
        assert(camera!=NULL);
        //try {
        Point3D sampledPoint = sampler->sampleUnitSquare();
        // Assuming rotate = 0 and normal = (1,0,0);
        //ray.source[0] = center[0];
        //ray.source[1] = center[1] + (width/2.0 - j + sampledPoint[1] + 0.5)*dotSize;
        //ray.source[2] = center[2] + (height/2.0 - i + sampledPoint[0] + 0.5)*dotSize;
        ray.source = center + e2 * (width/2.0 - j + sampledPoint[1] + 0.5)*dotSize + e1 * (height/2.0 - i + sampledPoint[0] + 0.5)*dotSize;
        if(perspective)
            ray.direction = ray.source - camera->position;
        else
            ray.direction = e1.crossProd(e2) * rightHandSide;
            //ray.direction = camera->normal;
        Vector focalPoint = ray.source + ray.direction * ((camera->focalDis - ray.source[0])/(ray.direction[0])); // Assuming face x-Plus direction (1,0,0)
        //Point3D anotherSample = sampler->sampleUnitSquare();
        double iBias = static_cast <double> (rand()) / static_cast <double> (RAND_MAX), jBias = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
        Point3D apertureMove = Point3D(0, iBias, jBias) * dotSize;
        ray.source += apertureMove * camera->aperture;
        ray.direction = focalPoint - ray.source;
        ray.direction.Normalize();
        return true;
        //} catch (int e) {
        //    return false;
        //}
    }
    bool getDirectionWithSampler(int i, int j, Ray & ray)
    {
        assert(camera!=NULL);
        //try {
            Point3D sampledPoint = sampler->sampleUnitSquare();
            // Assuming rotate = 0 and normal = (1,0,0);
            //ray.source[0] = center[0];
            //ray.source[1] = center[1] + (width/2.0 - j + sampledPoint[1] + 0.5)*dotSize;
            //ray.source[2] = center[2] + (height/2.0 - i + sampledPoint[0] + 0.5)*dotSize;
            ray.source = center + e2 * (width/2.0 - j + sampledPoint[1] + 0.5)*dotSize + e1 * (height/2.0 - i + sampledPoint[0] + 0.5)*dotSize;
            if(perspective)
                ray.direction = ray.source - camera->position;
            else
                ray.direction = e1.crossProd(e2) * rightHandSide;
                //ray.direction = camera->normal;
            ray.direction.Normalize();
            return true;
        //} catch (int e) {
        //    return false;
        //}
    }
    bool getDirection(int i, int j, int iSample, int jSample, Ray & ray)
    {
        assert(camera!=NULL);
        //try {
            // Assuming rotate = 0 and normal = (1,0,0);
            //ray.source[0] = center[0];
            //ray.source[1] = center[1] + (width/2.0 - j + double(jSample + 0.5)/wn)*dotSize;
            //ray.source[2] = center[2] + (height/2.0 - i + double(iSample + 0.5)/hn)*dotSize;
            ray.source = center + e2 * (width/2.0 - j + double(jSample + 0.5)/wn)*dotSize + e1 * (height/2.0 - i + double(iSample + 0.5)/hn)*dotSize;
            if(perspective)
                ray.direction = ray.source - camera->position;
            else
                ray.direction = e1.crossProd(e2) * rightHandSide;
                //ray.direction = camera->normal;
            ray.direction.Normalize();
            return true;
        //} catch (int e) {
        //    return false;
        //}
    }
    void importDefault()
    {
        perspective = DEFAULT_PERSPECTIVE;
    }
    Frame(){
        rotate = 0; dotSize = DEFAULT_DOTSIZE; importDefault();
    }
    Frame(int height, int width):height(height), width(width), dotSize(DEFAULT_DOTSIZE) { importDefault(); }
    Frame(int height, int width, double dotSize):height(height), width(width), dotSize(dotSize) { importDefault(); }
    Frame(double length, double weight, double dotSize): height((int)(length/dotSize)), width((int)(weight/dotSize)), dotSize(dotSize) { importDefault(); }
    Frame(double length, double weight): height((int)(length/DEFAULT_DOTSIZE)), width((int)(weight/DEFAULT_DOTSIZE)), dotSize(DEFAULT_DOTSIZE) { importDefault(); }
    
};

#endif /* defined(__RayTracer__Frame__) */
