//
//  Plane.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Plane__
#define __RayTracer__Plane__

#include <stdio.h>
#include "Geometry.h"
#include "Defaults.h"
#include "Utils.h"
#include "Prefix.h"
#include <cmath>
#include <opencv2/opencv.hpp>

class Plane: public Geometry
{
public:
    Plane() {};
    Plane(const Point3D p, const Vector & n): point(p), normal(n) {}
    Plane(const Point3D p, const Vector & n, Material *m): point(p), normal(n), Geometry(m) {}
    virtual bool hit(const Ray& ray, double &t, ShadeRec & sr);
    Point3D projectTo(Point3D p)
    {
        normal.Normalize();
        double distance = (p - point) * normal;
        return p - normal * distance;
    }
    bool onPlane(Point3D p)
    {
        return nearZero((p-point)*normal);
    }
    virtual RGBColor getColor() {
        return color;
    }
    virtual RGBColor getColor(Point3D p) {
        return color;
    }
protected:
    Point3D point;
    Vector normal;
    static const double Epsilon;
};

class GridPlane: public Plane
{
public:
    GridPlane(){};
    void initial()
    {
        relativePoint = projectTo(Point3D(0,0,0));
        //assert(!point==relativePoint);
        e1 = relativePoint - point;
        e1.Normalize();
        e2 = e1.crossProd(normal);
    }
    GridPlane(double g): gridLength(g) {
        initial();
    };
    GridPlane(const Point3D p, const Vector & n, double g): Plane(p, n), gridLength(g) {
        initial();
    }
    GridPlane(const Point3D p, const Vector & n, Material *m, double g): Plane(p, n, m), gridLength(g) {
        initial();
    }
    virtual RGBColor getColor(Point3D p)
    {
        //if(!onPlane(p)) p = projectTo(p);
        double xC = (p-relativePoint) * e1, yC = (p-relativePoint) * e2;
        return (((std::remainder(xC, gridLength)>0)) ^ (std::remainder(yC, gridLength)>0))? WHITE: BLACK;
    }
protected:
    double gridLength;
    Point3D relativePoint;
    Vector e1, e2;
};

class ImagePlane: public GridPlane
{
public:
    cv::Mat Img;
    int xBias, yBias;
    bool repeat;
    void Initial()
    {
         xBias = 0; yBias = 0; repeat=false;
    }
    ImagePlane(): GridPlane(0.1){repeat=false;};
    ImagePlane(double g, cv::Mat m): GridPlane(g), Img(m) {
        repeat=false;
    };
    ImagePlane(const Point3D p, const Vector & n, double g, cv::Mat m):
        GridPlane(p, n, g), Img(m)
    {repeat=false;}
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat): GridPlane(p, n, m, g), Img(mat) {
        repeat=false;
    }
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat, Vector & eX, Vector & eY): GridPlane(p, n, m, g), Img(mat) {
        e1 = eX; e2 = eY;
        Initial();
    }
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat, Vector eX, Vector eY): GridPlane(p, n, m, g), Img(mat) {
        e1 = eX; e2 = eY;
        Initial();
    }
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat, Vector & eX, Vector & eY, int xBias, int yBias): GridPlane(p, n, m, g), Img(mat), xBias(xBias), yBias(yBias) {
        e1 = eX; e2 = eY;
    }
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat, Vector eX, Vector eY, int xBias, int yBias): GridPlane(p, n, m, g), Img(mat), xBias(xBias), yBias(yBias) {
        e1 = eX; e2 = eY;
    }
    ImagePlane(const Point3D p, const Vector & n, double g, cv::Mat mat, Vector eX, Vector eY, int xBias, int yBias, bool repeat): GridPlane(p, n, g), Img(mat), xBias(xBias), yBias(yBias), repeat(repeat) {
        e1 = eX; e2 = eY;
    }
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat, Vector & eX, Vector & eY, int xBias, int yBias, bool repeat): GridPlane(p, n, m, g), Img(mat), xBias(xBias), yBias(yBias), repeat(repeat) {
        e1 = eX; e2 = eY;
    }
    ImagePlane(const Point3D p, const Vector & n, Material *m, double g, cv::Mat mat, Vector eX, Vector eY, int xBias, int yBias, bool repeat): GridPlane(p, n, m, g), Img(mat), xBias(xBias), yBias(yBias), repeat(repeat) {
        e1 = eX; e2 = eY;
    }
    virtual RGBColor getColor(Point3D p)
    {
        int xC = ((p-relativePoint) * e1 / gridLength) + xBias, yC = int((p-relativePoint) * e2 / gridLength) + yBias;
        if(!repeat)
        {
            if(xC<0 || yC<0 || xC >= Img.rows || yC >= Img.cols)
                return WHITE;
        }
        else
        {
            xC %= Img.rows; yC %= Img.cols;
            if(xC<0)xC+=Img.rows; if(yC<0)yC+=Img.cols;
        }
        cv::Vec3b pixel =Img.at<cv::Vec3b>(xC, yC);
        return Vector(pixel[2],pixel[1], pixel[0]);
    }
};

#endif /* defined(__RayTracer__Plane__) */
