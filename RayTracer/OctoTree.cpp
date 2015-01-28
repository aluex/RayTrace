//
//  OctoTree.cpp
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#include "OctoTree.h"
#include "Utils.h"
#include <iostream>
#include <cstring>

int OctoTree::maxFaceNum = 6;

void OctoTree::init()
{
    adjustBox();
    memset(children, NULL, sizeof(children));
    if(triList.size()>maxFaceNum)
        refine();
}

bool OctoTree::hit(const Ray &r, double &tMin, ShadeRec &sr)
{
    if(!testPassing(r))return false;
    ShadeRec tmpSr(sr.scene); double tmpT; tMin = HUGE_double;
    bool notFound = true;
    if(children[0])
    {
        
        for(int i = 0; i<8; ++i)
            if(children[i]->hit(r, tmpT, tmpSr) && tmpT < tMin && tmpT > 0)
            {
                tMin = tmpT;
                sr = tmpSr;
                notFound = false;
                ///////////////
            }
    }
    else
    {
        for(std::vector<Triangle*>::iterator it = triList.begin(); it!=triList.end(); ++it)
        if((*it)->hit(r, tmpT, tmpSr) && tmpT < tMin && tmpT > 0)
        {
            tMin = tmpT;
            sr = tmpSr;
            notFound = false;
        }
    }
    if(notFound) return false;
    return true;
}

bool OctoTree::testPassing(const Ray &r) const
{
    //if(r.source.near(p1) && r.source.near(p2))
    //    return true;
    if(r.source[0] > p1[0] - DEFAULT_EPSILON && r.source[1] > p1[1] - DEFAULT_EPSILON && r.source[2] > p1[2] - DEFAULT_EPSILON && r.source[0] < p2[0] + DEFAULT_EPSILON && r.source[1] < p2[1] + DEFAULT_EPSILON && r.source[2] < p2[2] + DEFAULT_EPSILON)
        return true; // Ray starts within the box;
    double minS[3], maxS[3];
    minS[0] = nearZero(r.direction[0])?-HUGE_double:(p1[0] - r.source[0])/r.direction[0];
    maxS[0] = nearZero(r.direction[0])?HUGE_double:(p2[0] - r.source[0])/r.direction[0];
    if(minS[0]>maxS[0])swap(minS[0], maxS[0]);
    minS[1] = nearZero(r.direction[1])?-HUGE_double:(p1[1] - r.source[1])/r.direction[1];
    maxS[1] = nearZero(r.direction[1])?HUGE_double:(p2[1] - r.source[1])/r.direction[1];
    if(minS[1]>maxS[1])swap(minS[1], maxS[1]);
    minS[2] = nearZero(r.direction[2])?-HUGE_double:(p1[2] - r.source[2])/r.direction[2];
    maxS[2] = nearZero(r.direction[2])?HUGE_double:(p2[2] - r.source[2])/r.direction[2];
    if(minS[2]>maxS[2])swap(minS[2], maxS[2]);
    double* minMax = std::min_element(maxS, maxS+3);
    double* maxMin = std::max_element(minS, minS+3);
    if(*maxMin<0 || *maxMin > *minMax) return false;
    return true;
}

bool OctoTree::cmpX(Triangle *a, Triangle *b)
{
    return (a->p[0][0] + a->p[1][0] + a->p[2][0])/3 < (b->p[0][0] + b->p[1][0] + b->p[2][0])/3;
}

bool OctoTree::cmpY(Triangle *a, Triangle *b)
{
    return (a->p[0][1] + a->p[1][1] + a->p[2][1])/3 < (b->p[0][1] + b->p[1][1] + b->p[2][1])/3;
}

bool OctoTree::cmpZ(Triangle *a, Triangle *b)
{
    return (a->p[0][2] + a->p[1][2] + a->p[2][2])/3 < (b->p[0][2] + b->p[1][2] + b->p[2][2])/3;
}

void OctoTree::refine()
{
    std::vector<Triangle*> xComponent[2], yComponent[2], zComponent[2];
    std::sort(triList.begin(), triList.end(), cmpX);
    xComponent[0] = std::vector<Triangle*>(triList.begin(), triList.begin() + (triList.size()>>1));
    xComponent[1] = std::vector<Triangle*>(triList.begin() + (triList.size()>>1), triList.end());
    for(int x = 0; x<2; ++x)
    {
        std::sort(xComponent[x].begin(), xComponent[x].end(), cmpY);
        yComponent[0] = std::vector<Triangle*>(xComponent[x].begin(), xComponent[x].begin() + (xComponent[x].size()>>1));
        yComponent[1] = std::vector<Triangle*>(xComponent[x].begin() + (xComponent[x].size()>>1), xComponent[x].end());
        for(int y = 0; y < 2; ++y)
        {
            std::sort(yComponent[y].begin(), yComponent[y].end(), cmpZ);
            zComponent[0] = std::vector<Triangle*>(yComponent[y].begin(), yComponent[y].begin() + (yComponent[y].size()>>1));
            zComponent[1] = std::vector<Triangle*>(yComponent[y].begin() + (yComponent[y].size()>>1), yComponent[y].end());
            for(int z = 0; z < 2; ++z)
            {
                children[(x<<2) + (y<<1) + z] = new OctoTree(zComponent[z]);
            }
        }
    }
    triList.clear();
}

void OctoTree::adjustBox() // Adjust to proper size
{
    if(!triList.empty())
    {
        for(int j=0; j<3; ++j)
        {
            double minF=HUGE_double, maxF = -HUGE_double;
            for(std::vector<Triangle*>::iterator it = triList.begin(); it!=triList.end(); ++it)
            for(int i=0; i<3;++i)
            {
                double f = (*it)->p[i][j];
                if(f<minF) minF = f;
                if(f>maxF) maxF = f;
            }
            p1[j] = minF - DEFAULT_EPSILON; p2[j] = maxF + DEFAULT_EPSILON;
        }
    }
}