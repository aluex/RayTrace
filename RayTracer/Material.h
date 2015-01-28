//
//  Material.h
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Material__
#define __RayTracer__Material__

#include <stdio.h>
#include "Prefix.h"
#include <opencv2/opencv.hpp>
//#include "tiny_obj_loader.h"

class Material
{
public:
    double refractionFact, reflectionFact, specularPower, N, glossyFact;
    Vector diffuseFact, specularFact, ambientFact;
    cv::Mat texture;
    bool withTexture;
    Material(): reflectionFact(0.4), N(1), refractionFact(0),
    specularFact(1,1,1), specularPower(5), diffuseFact(1,1,1), ambientFact(0,0,0), glossyFact(100), withTexture(false) {}
    // By default no glossy fact.
    /*
     Material( tinyobj::material_t &m ):
    ambientFact(m.ambient[0], m.ambient[1], m.ambient[2]),
    diffuseFact(m.diffuse[0], m.diffuse[1], m.diffuse[2]),
    specularFact(m.specular[0], m.specular[1], m.specular[2])
    {}
     */
};

class BasicMaterial: public Material
{
public:
    BasicMaterial():Material() {}; //By Default it is a solid
};

#endif /* defined(__RayTracer__Material__) */
