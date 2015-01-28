//
//  Prefix.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef RayTracer_Prefix_h
#define RayTracer_Prefix_h

#include "Vec3f.h"
#include <opencv2/opencv.hpp>
#include <utility>
#include "Ray.h"

//typedef cv::Vec3f Vector;
//typedef cv::Vec3f Point3D;
//typedef cv::Vec3f RGBColor;
typedef SimpleOBJ::Vec3f Vector;
typedef SimpleOBJ::Vec3f Point3D;
typedef SimpleOBJ::Vec3f RGBColor;
//typedef cv::Vec3b RGBColor;
typedef std::tuple<Ray, double, double, int> restrictRay; // Ray, reflectFactor, N, depth
#endif
