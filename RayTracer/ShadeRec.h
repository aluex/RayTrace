//
//  ShadeRec.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__ShadeRec__
#define __RayTracer__ShadeRec__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Prefix.h"
#include "Vec3f.h"
#include "Defaults.h"


class Scene;

class Geometry;

class ShadeRec {
public:
    bool hit;
    SimpleOBJ::Vec3f hitPoint;
    SimpleOBJ::Vec3f normal;
    RGBColor color;
    Scene *scene;
    Geometry* hitObject;
    ShadeRec(Scene *s): scene(s), hit(false), hitPoint(), normal(), color(DEFAULT_COLOR) {}
    ShadeRec(const ShadeRec & sr)
    : hit(sr.hit), hitPoint(sr.hitPoint), normal(sr.normal), color(sr.color), scene(sr.scene), hitObject(sr.hitObject) {}
};

#endif /* defined(__RayTracer__ShadeRec__) */
