//
//  Utils.h
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Utils__
#define __RayTracer__Utils__

#include <stdio.h>
#include <cmath>
#include "Prefix.h"

int inline min(int a, int b);

double inline min(double a, double b);

int inline min(int & a, int & b);

double inline min(double & a, double & b);

void inline swap(double &a, double &b)
{
    double t = a; a = b; b= t;
}

double getRandomDouble();

bool nearZero(double x);

Vector inline doRotate(Vector & origin, Vector &axis, double degree)
{
    double & x = axis[0], & y = axis[1], & z = axis[2], cosT = cosf(degree), sinT = sinf(degree), & a = origin[0], & b = origin[1], & c = origin[2];
    return SimpleOBJ::Vec3f(
        (cosT + (1-cosT)*x*x) * a + ((1- cosT) *x * y - sinT * z) * b + ((1-cosT)*x*z + sinT * y) * c,
        ((1-cosT)*y*x + sinT * z) * a + (cosT + (1-cosT)*y*y)*b + ((1-cosT)*y*z - sinT * x) * c,
        ((1-cosT)*z*x-sinT *y) * a + ((1-cosT)*z * y + sinT * x) * b + (cosT + (1-cosT)*z * z) * c
   );
}

Vector inline doRotate(Vector origin, Vector &axis, double degree)
{
    double & x = axis[0], & y = axis[1], & z = axis[2], cosT = cos(degree), sinT = sin(degree), & a = origin[0], & b = origin[1], & c = origin[2];
    return SimpleOBJ::Vec3f(
                            (cosT + (1-cosT)*x*x) * a + ((1- cosT) *x * y - sinT * z) * b + ((1-cosT)*x*z + sinT * y) * c,
                            ((1-cosT)*y*x + sinT * z) * a + (cosT + (1-cosT)*y*y)*b + ((1-cosT)*y*z - sinT * x) * c,
                            ((1-cosT)*z*x-sinT *y) * a + ((1-cosT)*z * y + sinT * x) * b + (cosT + (1-cosT)*z * z) * c
                            );
}

#endif /* defined(__RayTracer__Utils__) */
