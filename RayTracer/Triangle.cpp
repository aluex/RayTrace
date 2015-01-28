//
//  Triangle.cpp
//  RayTracer
//
//  Created by Aluex on 1/21/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#include "Triangle.h"
#include <opencv2/opencv.hpp>
#include "Utils.h"
#include "Prefix.h"

bool Triangle::hit(const Ray& ray, double &min, ShadeRec & sr)
{
    return _hit(ray, min, sr, 0);
}

bool Triangle::_hit(const Ray& ray, double &min, ShadeRec & sr, int testCount) const
{
    if(testCount>2)return false;
    //Referred from http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    cv::Mat A = (cv::Mat_<double>(3,3) <<
                 ray.direction[0], p[0][0] - p[1][0], p[0][0] - p[2][0],
                 ray.direction[1], p[0][1] - p[1][1], p[0][1] - p[2][1],
                 ray.direction[2], p[0][2] - p[1][2], p[0][2] - p[2][2]);
    A = A.inv();
    cv::Mat b = (cv::Mat_<double>(3,1) <<
                 p[0][0] - ray.source[0],
                 p[0][1] - ray.source[1],
                 p[0][2] - ray.source[2]);
    cv::Mat res = A * b; // Solution for (o + t * d) = u p1 + v p2 + (1-u-v) p3;
    min = res.at<double>(0,0);
    double u = res.at<double>(0,1);
    double v = res.at<double>(0,2);
    if(u < 0 || u > 1 || v < 0 || v > 1 || u + v > 1) return false;
    if(nearZero(min)) // On the triangle;
    {
        // Quick Hack By Blah-Geek
        Ray testRay(ray.source + ray.direction * DEFAULT_EPSILON, ray.direction);
        return _hit(testRay, min, sr, testCount+1);
    }
    if(min<0)
        return false;
    sr.hitPoint = ray.source + ray.direction * min;
    if(smoothNormal)
        sr.normal = n[0] * u+ n[1] * v + n[2] * (1-u-v);
    else
        sr.normal = (p[0]-p[1]).crossProd(p[0]-p[2]);
    sr.hitObject = (Geometry*)this;
    if(material->withTexture)
    {
        Vector pixelCoordinate = textureCoordinate[0] * u + textureCoordinate[1] * v + textureCoordinate[2] * (1-u-v);
        cv::Vec3b pixel =material->texture.at<cv::Vec3b>(int(pixelCoordinate[0] * material->texture.rows), int(pixelCoordinate[1] * material->texture.cols));
        sr.color = Vector(pixel[2],pixel[1], pixel[0]);
        //sr.color = material->texture.at<
    }
    else
        sr.color = sr.hitObject->getColor(sr.hitPoint);
        //sr.color = sr.hitObject->getColor(); // Call parent's getColor
    if(!rightHandNorm) sr.normal = -sr.normal;
    return true;
}