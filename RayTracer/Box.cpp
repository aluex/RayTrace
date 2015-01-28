//
//  Box.cpp
//  RayTracer
//
//  Created by Aluex on 1/21/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#include "Box.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>

bool Box::hit(const Ray &r, double &min, ShadeRec & sr)
{
    if(r.source.near(p1) && r.source.near(p2))
    {
        sr.hitPoint = p1;
        sr.normal = -r.direction;
        return true;
    }
    double minS[3], maxS[3];
    minS[0] = nearZero(r.direction[0])?-HUGE_double:(p1[0] - r.source[0])/r.direction[0];
    maxS[0] = nearZero(r.direction[0])?HUGE_double:(p2[0] - r.source[0])/r.direction[0];
    if(minS[0]>maxS[0])swap(minS[0], maxS[0]);
    minS[1] = nearZero(r.direction[1])?-HUGE_double:(p1[1] - r.source[1])/r.direction[1];
    maxS[1] = nearZero(r.direction[1])?HUGE_double:(p2[1] - r.source[1])/r.direction[1];
    if(minS[1]>maxS[1])swap(minS[1], maxS[1]);
    minS[2] = nearZero(r.direction[2])?-HUGE_double:(p1[2] - r.source[2])/r.direction[2];
    maxS[2] = nearZero(r.direction[2])?HUGE_double:(p2[2] - r.source[2])/r.direction[2];
    if(minS[2]>maxS[2])swap(minS[2], maxS[2]);
    double* minMax = std::min_element(maxS, maxS+3);
    double* maxMin = std::max_element(minS, minS+3);
    if(*maxMin<0 || *maxMin > *minMax) return false;
    sr.hitPoint = r.source + r.direction * *maxMin;
    sr.hitObject = (Geometry*)this;
    sr.color = sr.hitObject->getColor(sr.hitPoint);
    //sr.color = sr.hitObject->getColor();
    switch(maxMin - minS) // Assuming double-sided!
    {
        case 0: sr.normal = Vector(1,0,0); break;
        case 1: sr.normal = Vector(0,1,0); break;
        case 2: sr.normal = Vector(0,0,1); break;
    }
    return true;
}