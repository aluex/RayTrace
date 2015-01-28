//
//  Sphere.cpp
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//
#include <iostream>
#include "Sphere.h"
#include "Defaults.h"

double Sphere::Epsilon = DEFAULT_EPSILON;

bool Sphere::hit(const Ray& ray, double &min, ShadeRec & sr)
{
#ifdef RAY_DEBUG
//    std::cout << "Sphere hit check:" << ray << std::endl;
#endif
    Vector vtmp = (ray.source - center);
    double c = vtmp * vtmp - radius * radius, bHalf = vtmp * ray.direction, a = ray.direction * ray.direction;
    double delta = bHalf*bHalf - a * c;
    if (delta < 0) return false;
    
    double e = sqrtf(delta);
    double tmp = (-e-bHalf)/a;
    if(tmp > Epsilon)
    {
        min = tmp;
        sr.normal = vtmp + ray.direction * min;
        sr.hitPoint = ray.source + ray.direction * min;
        sr.hitObject = (Geometry*)this;
        //sr.color = sr.hitObject->getColor();
        sr.color = sr.hitObject->getColor(sr.hitPoint);
        return true;
    }
    tmp = (e-bHalf)/a;
    if(tmp > Epsilon)
    {
        min = tmp;
        sr.normal = - (vtmp + ray.direction * min); /// Inside the Sphere
        sr.hitPoint = ray.source + ray.direction * min;
        sr.hitObject = (Geometry*)this;
        //sr.color = sr.hitObject->getColor();
        sr.color = sr.hitObject->getColor(sr.hitPoint);
        return true;
    }
    return false;
};
