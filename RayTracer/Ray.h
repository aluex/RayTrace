//
//  Ray.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Ray__
#define __RayTracer__Ray__

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Vec3f.h"

class Ray
{
public:
    SimpleOBJ::Vec3f source, direction;
    Ray(){}
    Ray(SimpleOBJ::Vec3f s, SimpleOBJ::Vec3f d): source(s), direction(d) {}
    Ray(const Ray &ray){
        source = ray.source; direction = ray.direction;
    }
    Ray & operator=(const Ray & rhs){
        source = rhs.source; direction = rhs.direction;
        return *this;
    }
    ~Ray(void){}
    friend std::ostream & operator<<(std::ostream & os, const Ray & r)
    {
        os << "(" << r.source << "--" << r.direction << ")";
        return os;
    }
};

#endif /* defined(__RayTracer__Ray__) */
