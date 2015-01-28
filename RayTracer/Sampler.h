//
//  Sampler.h
//  RayTracer
//
//  Created by Aluex on 1/20/15.
//  Copyright (c) 2015 Aluex. All rights reserved.
//

#ifndef __RayTracer__Sampler__
#define __RayTracer__Sampler__

#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Prefix.h"

class Sampler
{
public:
    int capacity;
    std::vector<Point3D> samples;
    std::vector<int> shuffledIndices;
    int count, setIndex;
    int numSamples;
    //static int hn, wn;
    Sampler(int capacity, int numS): capacity(capacity), numSamples(numS) { count = 0; setIndex = 0;}
    virtual void generateSamples() = 0;
    virtual Point3D sampleUnitSquare() = 0;
    virtual void shuffleSamples() = 0;
    virtual void setupShuffledIndices() = 0;
    ~Sampler(){}
};

class PureRandomSampler: public Sampler
{
public:
    PureRandomSampler():Sampler(4, 1) {}
    PureRandomSampler(int capacity, int ns): Sampler(capacity, ns) {}
    virtual Point3D sampleUnitSquare() {
        return Point3D(static_cast <double> (rand()) / static_cast <double> (RAND_MAX), static_cast <double> (rand()) / static_cast <double> (RAND_MAX), 0);
    }
    virtual void setupShuffledIndices() {}
    virtual void shuffleSamples(){}
    virtual void generateSamples(){}
};

class JitterSampler: public Sampler
{
public:
    JitterSampler():Sampler(4, 1) {}
    JitterSampler(int capacity, int ns): Sampler(capacity, ns) {}
    virtual Point3D sampleUnitSquare() {
        if(count % capacity ==0)
            setIndex = (rand() % numSamples) * capacity;
        return samples[setIndex + shuffledIndices[setIndex + count++%capacity]];
    }
    virtual void setupShuffledIndices() {
        shuffledIndices.reserve(capacity * numSamples);
        std::vector<int> indices;
        for(int i=0; i< capacity; ++i)
            indices.push_back(i);
        for(int p=0; p< numSamples; ++p)
        {
            std::random_shuffle(indices.begin(), indices.end());
            for(int j=0; j<capacity; ++j)
                shuffledIndices.push_back(indices[j]);
                
        }
    }
    virtual void shuffleSamples(){}
    virtual void generateSamples(){
        int n = (int) sqrt(capacity);
        assert(n*n==capacity); // capacity should be a squared num;
        
        for(int k=0; k< numSamples; ++k)
        {
            for(int i=0; i<n; ++i)
                for(int j=0; j<n; ++j)
                {
                    double iBias = static_cast <double> (rand()) / static_cast <double> (RAND_MAX), jBias = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
                    Point3D p((i+iBias)/n, (j+jBias)/n, 0);
                    samples.push_back(p);
                }
        }
        samples.reserve(capacity * numSamples);
    }
};

#endif /* defined(__RayTracer__Sampler__) */
