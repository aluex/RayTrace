//
//  Light.h
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Light__
#define __RayTracer__Light__

#include <stdio.h>
#include "Prefix.h"
#include "Sampler.h"
#include "Utils.h"

class Light
{
public:
    Point3D position;
    RGBColor color;
    Sampler *sampler;
    double intensity;
    double range; // The radius of area of the light
    Light():color(255,255,255){}
    Light(Point3D position, RGBColor color, double intensity, double range):
    position(position), color(color), intensity(intensity), range(range), sampler(NULL)
    { }
    Light(Point3D position, RGBColor color, double intensity, double range, Sampler *sampler):
        position(position), color(color), intensity(intensity), range(range), sampler(sampler)
    { }
    virtual Point3D getPosition()
    {
        return position + Vector(getRandomDouble(), getRandomDouble(), getRandomDouble()) * range;
    }
};

#endif /* defined(__RayTracer__Light__) */
