//
//  Geometry.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Geometry__
#define __RayTracer__Geometry__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Ray.h"
#include "Vec3f.h"
#include "Defaults.h"
#include "ShadeRec.h"
#include "Material.h"

class Geometry
{
public:
    RGBColor color;
    Material *material;
    bool bothSide;
    Geometry(): color(DEFAULT_COLOR), material(new BasicMaterial()), bothSide(true) {}
    Geometry(Material *m): color(DEFAULT_COLOR), material(m), bothSide(true) {}
    Geometry(RGBColor color): color(color), material(new BasicMaterial()), bothSide(true) {}
    Geometry(RGBColor color, Material *mat): color(color), material(mat), bothSide(true) {}
    virtual RGBColor getColor() {
        return color;
    }
    virtual RGBColor getColor(Point3D p) {
        return color;
    }
    //void setColor(RGBColor & c) {color = c;}
    void setColor(RGBColor c) {color = c;}
    virtual bool hit(const Ray &r, double &min, ShadeRec & sr) = 0;
};

#endif /* defined(__RayTracer__Geometry__) */
