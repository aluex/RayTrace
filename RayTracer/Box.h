//
//  Box.h
//  RayTracer
//
//  Created by Aluex on 1/21/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Box__
#define __RayTracer__Box__

#include <stdio.h>
#include "Geometry.h"

class Box: public Geometry
{
public:
    Point3D p1, p2;
    Box(Point3D p1, Point3D p2):p1(p1), p2(p2) {}
    Box(Point3D p1, Point3D p2, Material *m):p1(p1), p2(p2), Geometry(m) {}
    /*double intersectGadget(double a, double b, double c)
    {
        return (b - a)/c;
    }*/
    virtual bool hit(const Ray &r, double &min, ShadeRec & sr);
};

#endif /* defined(__RayTracer__Box__) */
