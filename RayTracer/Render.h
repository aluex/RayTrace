//
//  Render.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Render__
#define __RayTracer__Render__

#include <stdio.h>
#include <cassert>
#include "Frame.h"
#include "Scene.h"
#include "Defaults.h"
#include "Utils.h"
#include <vector>
#include <cmath>
#include <queue>
#include <opencv2/opencv.hpp>

class Render
{
    cv::Mat buffer;
    int height;
    int width;
public:
    double invGamma;
    int hn, wn;
    int MAX_DEPTH;
    double secondRayControl;
    int extraReflectionTest;
    Frame *frame;
    Scene *scene;
    Render(cv::Mat m, int height, int width, int hn, int wn, double invGamma, int MAX_DEPTH, double secondRayControl, int extraReflectionTest): buffer(m), height(height), width(width), hn(hn), wn(wn), invGamma(invGamma), MAX_DEPTH(MAX_DEPTH), secondRayControl(secondRayControl), extraReflectionTest(extraReflectionTest)
    {
        //////
    }
    
    bool rayTrace(Ray &ray, double & tRes, ShadeRec & resSr)
    {
        double tMin = HUGE_double, t;
        bool notFound = true;
        ShadeRec tmpSr(resSr.scene);
        tmpSr.color = BLACK;
        //ray.direction.Normalize();
        std::vector<Geometry*>::iterator it = scene->objList.begin();
        for(;it!=scene->objList.end();++it)
            if((*it)->hit(ray, t, tmpSr) && t<tMin && t > 0){
                tMin = t;
                notFound = false;
                resSr = tmpSr;
                //resSr.hitObject = *it; // Set by the hit function.
                //resSr.color = (*it)->getColor(tmpSr.hitPoint);
                //resSr.color = tmpSr.hitObject->getColor(tmpSr.hitPoint);
            }
        if(notFound)return false;
        tRes = tMin;
        return true;
    }
    RGBColor gammaAdjust(RGBColor &color)
    {
        RGBColor resColor(color);
        resColor[0] = min(int(pow(resColor[0], invGamma)), 255);
        resColor[1] = min(int(pow(resColor[1], invGamma)), 255);
        resColor[2] = min(int(pow(resColor[2], invGamma)), 255);
        return resColor;
    }
    void renderPixel(int i, int j)
    {
        cv::Vec3b & color = buffer.at<cv::Vec3b>(i,j);
        Ray ray; double tMin;
        Vector colorFinal(0,0,0);
        for(int iSample=0; iSample<hn; ++iSample)
            for(int jSample=0; jSample<wn; ++jSample)
            {
                frame->getDirectionWithSampler(i, j, ray);
                //frame->getDirectionWithSamplerWithDOF(i, j, ray);
                ////////////
                std::vector<restrictRay> rayQueue;
                rayQueue.push_back(restrictRay(ray, 1.0, 1.0, 0));
                RGBColor colorPhong(0,0,0);
                do{
                    restrictRay currentRay = rayQueue.back();
                    rayQueue.pop_back();
                    ShadeRec tmpSr(scene), newSr(scene);
                    Ray &ray = std::get<0>(currentRay);
                    double currentFactor = std::get<1>(currentRay), N = std::get<2>(currentRay);
                    int depth = std::get<3>(currentRay);
                    if(rayTrace(ray, tMin, tmpSr))
                    {
                        tmpSr.normal.Normalize();
                        Vector V = ray.direction;
                        if(tmpSr.hitObject->bothSide && tmpSr.normal * V > 0)
                            tmpSr.normal = - tmpSr.normal;
                        V.Normalize();
                        Material *currentMaterial = tmpSr.hitObject->material;
                        Vector tmpColor(tmpSr.color.prodEWise(currentMaterial->ambientFact));
                        for(std::vector<Light*>::iterator it=
                            scene->lightList.begin(); it!=scene->lightList.end(); ++it)
                        {
                            //Vector L = (*it)->position - tmpSr.hitPoint;
                            Vector L = (*it)->getPosition() - tmpSr.hitPoint;
                            Ray reflectRay(tmpSr.hitPoint, L);
                            if(reflectRay.direction * tmpSr.normal > 0)
                            {
                                bool cross = rayTrace(reflectRay, tMin, newSr);
                                if(cross && tMin < 1.0) continue; // The light is blocked
                                L.Normalize();
                                Vector R = tmpSr.normal * (2 * (L * tmpSr.normal)) - L;
                                double specularBase = -R * V;
                                if(specularBase<=0)
                                    tmpColor += tmpSr.color.prodEWise(currentMaterial->diffuseFact) * (*it)->intensity * (L* tmpSr.normal);
                                else
                                    tmpColor += tmpSr.color.prodEWise(currentMaterial->diffuseFact) * (*it)->intensity * (L* tmpSr.normal)
                                    + currentMaterial->specularFact.prodEWise((*it)->color) * (*it)->intensity*pow(-R * V, currentMaterial->specularPower);
                            }
                        }
                        colorPhong += tmpColor * currentFactor;
                        if(depth < MAX_DEPTH)
                        {
                            double nextRefractionFactor = currentFactor * currentMaterial->refractionFact;
                            if(tmpSr.hitObject->bothSide && tmpSr.normal * V > 0)
                                tmpSr.normal = -tmpSr.normal; // Both side can reflect or refract;
                            Vector reflectDirection = (tmpSr.normal * (2 * (-V * tmpSr.normal)) + V);
                            //double nextReflectionFactor =currentFactor * currentMaterial->reflectionFact;
                            double nextReflectionFactor =currentFactor * currentMaterial->reflectionFact;
                            if(nextReflectionFactor > DEFAULT_EPSILON)
                            {
                                Ray reflectRay(tmpSr.hitPoint, reflectDirection * secondRayControl);
                                rayQueue.push_back(restrictRay(reflectRay, nextReflectionFactor, N, depth+1));
                            }
                            if(nextRefractionFactor > DEFAULT_EPSILON)
                            {
                                double matN = 1.0/currentMaterial->N, cosI = - tmpSr.normal * V;
                                double cosJSQ = 1 - matN*matN *(1 - cosI * cosI);
                                if(cosJSQ>0) // Otherwise complete reflection
                                {
                                    Vector T = tmpSr.normal * (matN * cosI - sqrt(cosJSQ)) + V * matN;
                                    rayQueue.push_back(restrictRay(Ray(tmpSr.hitPoint, T * secondRayControl), nextRefractionFactor, N * matN, depth+1));
                                }
                            }
                            for(int extraReflectionCounter = 0; extraReflectionCounter < extraReflectionTest; ++ extraReflectionCounter)
                            {
                                double changeX = reflectDirection[0] *(getRandomDouble() + 0.5), changeY = reflectDirection[1] * (getRandomDouble() + 0.5), changeZ = reflectDirection[2] * (getRandomDouble() + 0.5);
                                Vector extraReflectRay(changeX, changeY, changeZ);
                                if(extraReflectRay * tmpSr.normal < 0)
                                    continue;
                                extraReflectRay.Normalize();
                                double extraReflectionFactor = nextReflectionFactor * pow(reflectDirection * extraReflectRay, currentMaterial->glossyFact);
                                assert(!std::isnan(extraReflectionFactor));
                                if(extraReflectionFactor > DEFAULT_EPSILON)
                                    rayQueue.push_back(restrictRay(
                                    Ray(tmpSr.hitPoint, extraReflectRay),
                                    extraReflectionFactor, N, depth+1));
                                /////
                            }
                        }
                    }
                }while(!rayQueue.empty());
                colorFinal += colorPhong.sigmoid();
            }
        colorFinal /= hn * wn;
        color = cv::Vec3b((uchar)colorFinal[2], (uchar)colorFinal[1], (uchar)colorFinal[0]);
    }
    void doRender()
    {
        assert(frame!=NULL && scene!=NULL);
        int count = 0, step = int(height * width * 0.01);
        //renderPixel(394, 210);
#pragma omp parallel for schedule(dynamic)
        for(int i=0; i<height; ++i)
            for(int j=0; j<width; ++j)
            {
                renderPixel(i,j);
#pragma omp atomic
                ++count;
                if(count%step==0)
                    printf("%d percent\n", count / step);
            }
    }
};

#endif /* defined(__RayTracer__Render__) */
