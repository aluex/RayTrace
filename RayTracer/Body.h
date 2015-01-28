//
//  Body.h
//  RayTracer
//
//  Created by Aluex on 1/22/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Body__
#define __RayTracer__Body__

#include <stdio.h>
#include "Prefix.h"
#include "Triangle.h"
#include "objLoader.h"
#include "Material.h"
#include "Utils.h"

class Body
{
public:
    std::vector<Triangle*> triList;
    Material *objMaterial;
    Material **objMaterialList;
    Vector objBias, rotateAxis;
    double size, rotateDegree;
    Vector color;
    Body(){}
    Body(char const *fileName, Vector objBias, double size, Vector rotateAxis, double rotateDegree, Vector color, Material *m)
    : objBias(objBias), size(size), rotateAxis(rotateAxis), rotateDegree(rotateDegree), objMaterial(m), color(color)
    {   this->rotateAxis.Normalize();
        loadFromFile(fileName); }
    ~Body()
    {
        
    }
    void loadFromFile(char const *fileName)
    {
        triList.clear();
        objLoader *objData  = new objLoader(); //Obj Loader From http://www.kixor.net/dev/objloader/, modified
        objData -> load(fileName);
        objMaterialList = new Material*[objData->materialCount];
        for(int i=0; i<objData->materialCount; ++i) // Load Materials
        {
            obj_material *om = objData->materialList[i];
            objMaterialList[i] = new BasicMaterial;
            objMaterialList[i]->ambientFact = Vector(
                                              double(om->amb[0]), double(om->amb[1]), double(om->amb[2])
                                              );
            objMaterialList[i]->diffuseFact = Vector(
                                              double(om->diff[0]), double(om->diff[1]), double(om->diff[2])
                                              );
            objMaterialList[i]->specularFact = Vector(
                                               double(om->spec[0]), double(om->spec[1]), double(om->spec[2])
                                               );
            objMaterialList[i]->reflectionFact = (double)om->reflect;
            objMaterialList[i]->refractionFact = (double)om->refract;
            objMaterialList[i]->N = om->refract_index;
            if((double)om->shiny)
            objMaterialList[i]->specularPower = (double)om->shiny;
            else objMaterialList[i]->specularPower = 100;
            if(om->texture_filename && om->texture_filename[0])
            {
                objMaterialList[i]->texture = cv::Mat(cvLoadImage(om->texture_filename));
                if(objMaterialList[i]->texture.rows && objMaterialList[i]->texture.cols) // Successfully read
                    objMaterialList[i]->withTexture = true;
            }
        }
        for(int i=0; i<objData->faceCount; ++i)
        {
            obj_face *o = objData->faceList[i];
            
            //if(o->vertex_count== o->normal_count)
            for(int j=2; j<o->vertex_count; ++j)
            {
                Triangle *t;
                Material *currentM = objMaterial;
                if(o->material_index!=-1) // Different material for different faces
                {
                    currentM = objMaterialList[o->material_index];
                }
                if(o->normal_index[0]!=-1)
                t = new Triangle(
                                       doRotate(Vector(-objData->vertexList[o->vertex_index[0]]->e[2],
                                              -objData->vertexList[o->vertex_index[0]]->e[0],
                                              objData->vertexList[o->vertex_index[0]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                       doRotate(Vector(-objData->vertexList[o->vertex_index[j-1]]->e[2],
                                                       -objData->vertexList[o->vertex_index[j-1]]->e[0],
                                                       objData->vertexList[o->vertex_index[j-1]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                       doRotate(Vector(-objData->vertexList[o->vertex_index[j]]->e[2],
                                                       -objData->vertexList[o->vertex_index[j]]->e[0],
                                                       objData->vertexList[o->vertex_index[j]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                       currentM,
                                           doRotate(Vector(-objData->normalList[o->normal_index[0]]->e[2],
                                                           -objData->normalList[o->normal_index[0]]->e[0],
                                                           objData->normalList[o->normal_index[0]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                           doRotate(Vector(-objData->normalList[o->normal_index[j-1]]->e[2],
                                                           -objData->normalList[o->normal_index[j-1]]->e[0],
                                                           objData->normalList[o->normal_index[j-1]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                           doRotate(Vector(-objData->normalList[o->normal_index[j]]->e[2],
                                                           -objData->normalList[o->normal_index[j]]->e[0],
                                                           objData->normalList[o->normal_index[j]]->e[1]), rotateAxis, rotateDegree) * size + objBias
                                           );
                else
                    t = new Triangle(
                                     doRotate(Vector(-objData->vertexList[o->vertex_index[0]]->e[2],
                                                     -objData->vertexList[o->vertex_index[0]]->e[0],
                                                     objData->vertexList[o->vertex_index[0]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                     doRotate(Vector(-objData->vertexList[o->vertex_index[j-1]]->e[2],
                                                     -objData->vertexList[o->vertex_index[j-1]]->e[0],
                                                     objData->vertexList[o->vertex_index[j-1]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                     doRotate(Vector(-objData->vertexList[o->vertex_index[j]]->e[2],
                                                     -objData->vertexList[o->vertex_index[j]]->e[0],
                                                     objData->vertexList[o->vertex_index[j]]->e[1]), rotateAxis, rotateDegree) * size + objBias,
                                     currentM);
                t->setColor(color);
                if(o->texture_index[0]!=-1)
                {
                    t->textureCoordinate[0] = Vector(
                        objData->textureList[o->texture_index[0]]->e[0],
objData->textureList[o->texture_index[0]]->e[1], objData->textureList[o->texture_index[0]]->e[2]);
                    t->textureCoordinate[1] = Vector(
                                                     objData->textureList[o->texture_index[j-1]]->e[0],
                                                     objData->textureList[o->texture_index[j-1]]->e[1], objData->textureList[o->texture_index[j-1]]->e[2]);
                    t->textureCoordinate[2] = Vector(
                                                     objData->textureList[o->texture_index[j]]->e[0],
                                                     objData->textureList[o->texture_index[j]]->e[1], objData->textureList[o->texture_index[j]]->e[2]);
                }
                triList.push_back(t);
            }
        }
    }
};

#endif /* defined(__RayTracer__Body__) */
