//
//  Plane.cpp
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#include "Defaults.h"
#include "ShadeRec.h"
#include "Ray.h"
#include "Plane.h"

const double Plane::Epsilon = DEFAULT_EPSILON;

bool Plane::hit(const Ray& ray, double &t, ShadeRec & sr)
{
    double tmp = (point - ray.source)*(normal)
    / (normal * (ray.direction));
    if(tmp > Epsilon)
    {
        t = tmp;
        if(normal * ray.direction >0) sr.normal = -normal;
        else sr.normal = normal;
        sr.hitPoint = ray.source + ray.direction * tmp;
        sr.hitObject = (Geometry*)this;
        sr.color = sr.hitObject->getColor(sr.hitPoint);
        //sr.color = sr.hitObject->getColor();
        return true;
    }
    return false;
}
