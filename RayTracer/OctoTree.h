//
//  OctoTree.h
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__OctoTree__
#define __RayTracer__OctoTree__

#include <stdio.h>
#include <iostream>
#include "Prefix.h"
#include "Triangle.h"

class OctoTree: public Geometry
{
public:
    //////////////// For OBJ File!!
    static int maxFaceNum;
    std::vector<Triangle*> triList;
    OctoTree *children[8];
    Point3D p1, p2;
    OctoTree(){}
    void init();
    OctoTree(const std::vector<Triangle*> t, Material *m): triList(t), Geometry(m) { init(); }
    OctoTree(const std::vector<Triangle*> t): triList(t) { init(); }
    //OctoTree(const std::vector<Triangle*> &t): triList(t) { init(); }
    bool testPassing(const Ray &r) const;
    void refine();
    void adjustBox();
    virtual bool hit(const Ray &r, double &tMin, ShadeRec &sr);
    static bool cmpX(Triangle *a, Triangle *b);
    static bool cmpY(Triangle *a, Triangle *b);
    static bool cmpZ(Triangle *a, Triangle *b);
};

#endif /* defined(__RayTracer__OctoTree__) */
