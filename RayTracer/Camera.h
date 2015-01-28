//
//  Camera.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Camera__
#define __RayTracer__Camera__

#include <stdio.h>
#include "Vec3f.h"
#include "Prefix.h"

class Scene;

class Camera
{
public:
    Point3D position;
    Vector normal;
    double focalDis;
    double aperture;
    Camera(){ position = Point3D(-1,0,0); normal = Point3D(1,0,0);} // Look towards X+ direction
    //Camera(Scene *s):scene(s){}
    Camera(Point3D &p, Vector &n): position(p), normal(n) {}
    
};

#endif /* defined(__RayTracer__Camera__) */
