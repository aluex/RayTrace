//
//  Scene.h
//  RayTracer
//
//  Created by Aluex on 1/19/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Scene__
#define __RayTracer__Scene__

#include <iostream>
#include <vector>
#include <exception>
#include "Geometry.h"
#include "Light.h"
class Scene
{
public:
    std::vector<Geometry*> objList;
    std::vector<Light*> lightList;
    Scene(){};
    bool addLight(Light *l)
    {
        //try {
        lightList.push_back(l);
        return true;
        //} catch (int e) {
        //    return false;
        //}
        
    }
    /*
    bool addLight(Light *&l)
    {
        //try {
            lightList.push_back(l);
            return true;
        //} catch (int e) {
        //    return false;
        //}
        
    }*/
    bool addObject(Geometry *&g)
    {
        //try {
            objList.push_back(g);
            return true;
        //} catch (int e) {
        //    return false;
        //}
        
    }
};

#endif /* defined(__RayTracer__Scene__) */
