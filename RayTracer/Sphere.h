//
//  Sphere.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Sphere__
#define __RayTracer__Sphere__

#include <stdio.h>
#include "Geometry.h"

class Sphere: public Geometry
{
public:
    Sphere(void){}
    Sphere(Point3D c, double r): center(c), radius(r) {}
    Sphere(Point3D & c, double r): center(c), radius(r) {}
    Sphere(Point3D c, double r, Material *m): center(c), radius(r), Geometry(m) {}
    Sphere(Point3D & c, double r, Material *m): center(c), radius(r), Geometry(m) {}
    virtual bool hit(const Ray& ray, double &min, ShadeRec & sr);
    virtual RGBColor getColor(Point3D p) {
        return color;
    }
    virtual RGBColor getColor() {
        return color;
    }
private:
    Point3D center;
    double radius;
    static double Epsilon;
};

#endif /* defined(__RayTracer__Sphere__) */
