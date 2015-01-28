//
//  Triangle.h
//  RayTracer
//
//  Created by Aluex on 1/21/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Triangle__
#define __RayTracer__Triangle__

#include <stdio.h>
#include "Geometry.h"

class Triangle: public Geometry
{
public:
    Point3D p[3];
    Point3D n[3];
    Point3D textureCoordinate[3];
    bool smoothNormal;
    bool rightHandNorm;
    //bool withTexture;
    Triangle(){ rightHandNorm = true;}
    Triangle(Point3D p1, Point3D p2, Point3D p3): rightHandNorm(true)
    { p[0]=p1; p[1]=p2; p[2]=p3; smoothNormal=false;}
    ///Triangle(Point3D & p1, Point3D & p2, Point3D & p3): rightHandNorm(true) { p[0]=p1; p[1]=p2; p[2]=p3; }
    Triangle(Point3D p1, Point3D p2, Point3D p3, Material *m): Geometry(m), rightHandNorm(true)
    { p[0]=p1; p[1]=p2; p[2]=p3; smoothNormal=false;}
    Triangle(Point3D p1, Point3D p2, Point3D p3, Material *m, Point3D n1, Point3D n2, Point3D n3): Geometry(m), rightHandNorm(true)
    {
        p[0]=p1; p[1]=p2; p[2]=p3;
        n[0]=n1; n[1]=n2; n[2]=n3; smoothNormal = true;
    }
    //Triangle(Point3D & p1, Point3D & p2, Point3D & p3, Material *m): Geometry(m), rightHandNorm(true) { p[0]=p1; p[1]=p2; p[2]=p3; }
    Triangle(Point3D p1, Point3D p2, Point3D p3, bool right): rightHandNorm(right) { p[0]=p1; p[1]=p2; p[2]=p3;rightHandNorm = true; smoothNormal=false; }
    
    //Triangle(Point3D & p1, Point3D & p2, Point3D & p3, bool right): rightHandNorm(right) { p[0]=p1; p[1]=p2; p[2]=p3;rightHandNorm = true; }
    virtual bool hit(const Ray& ray, double &min, ShadeRec & sr);
    bool _hit(const Ray& ray, double &min, ShadeRec & sr, int testCount) const;
    virtual RGBColor getColor(Point3D p) {
        return color;
    }
    virtual RGBColor getColor() {
        return color;
    }
};

#endif /* defined(__RayTracer__Triangle__) */
