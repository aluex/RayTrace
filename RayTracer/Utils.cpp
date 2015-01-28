//
//  Utils.cpp
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#include "Utils.h"
#include "Defaults.h"
#include <cmath>

int inline min(int a, int b)
{
    return a<b?a:b;
}

double inline min(double a, double b)
{
    return a<b?a:b;
}

int inline min(int & a, int & b)
{
    return a<b?a:b;
}

double inline min(double & a, double & b)
{
    return a<b?a:b;
}

bool nearZero(double x)
{
    return fabs(x) < DEFAULT_EPSILON;
}

double getRandomDouble()
{
    return static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
}
