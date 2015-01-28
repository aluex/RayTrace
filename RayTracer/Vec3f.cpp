//#include "StdAfx.h"
#include <iostream>
#include <cstring>
#include "Vec3f.h"
#include "Utils.h"
#include <cmath>

namespace SimpleOBJ
{

//////////////////////////////////////////////////////////////////////////
//  Constructors and Deconstructors
    Vec3f::Vec3f(void)
    {
        memset(_p,0,sizeof(double)*_len);
    }
    
    Vec3f::Vec3f(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vec3f::Vec3f(const Vec3f &v)
    {
        memcpy(_p,v._p,sizeof(double)*_len);
    }

    Vec3f::~Vec3f(void)
    {

    }

//////////////////////////////////////////////////////////////////////////
// Operators

    Vec3f& Vec3f::operator =( const Vec3f& v)
    {
        memcpy(_p,v._p,sizeof(double)*_len);        
        return (*this);
    }

    Vec3f& Vec3f::operator +=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] += v._p[i];
        return (*this);
    }
    
    /*
    Vec3f& Vec3f::operator +=(const Vec3f v)
    {
        for(int i=0;i<_len;i++)
            _p[i] += v._p[i];
        return *this;
    }*/
    
    Vec3f& Vec3f::operator +=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] += f;
        return *this;
    }

    void Vec3f::operator -=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] -= v._p[i];
    }
    
    void Vec3f::operator -=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] -= f;
    }

    void Vec3f::operator *=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] *= v._p[i];
    }
    void Vec3f::operator *=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] *= f;
    }

    void Vec3f::operator /=(const Vec3f& v)
    {
        for(int i=0;i<_len;i++)
            _p[i] /= v._p[i];
    }
    void Vec3f::operator /=(double f)
    {
        for(int i=0;i<_len;i++)
            _p[i] /= f;
    }

    Vec3f Vec3f::operator +(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] + v[i];
        return res;
    }
    Vec3f Vec3f::operator +(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] + f;
        return res;
    }

    Vec3f Vec3f::operator -(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] - v[i];
        return res;
    }
    Vec3f Vec3f::operator -(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] - f;
        return res;
    }

    double Vec3f::operator *(const Vec3f & v) const
    {
        return (*this)[0] *v[0] + (*this)[1] *v[1] + (*this)[2] *v[2];
    }
    Vec3f Vec3f::operator *(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] * f;
        return res;
    }

    Vec3f Vec3f::operator /(const Vec3f&v) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] / v[i];
        return res;
    }
    Vec3f Vec3f::operator /(double f) const
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = (*this)[i] / f;
        return res;
    }

    Vec3f Vec3f::operator - () const 
    {
        Vec3f res;
        for(int i=0;i<_len;i++)
            res[i] = -(*this)[i];
        return res;
    }
    
    bool Vec3f::operator==(const Vec3f & v)
    {
        return nearZero(v[0]-(*this)[0])
        && nearZero(v[1]-(*this)[1])
        && nearZero(v[2]-(*this)[2]);
    }
    
    bool Vec3f::near(const Vec3f & v) const
    {
        return nearZero(v[0]-(*this)[0])
        && nearZero(v[1]-(*this)[1])
        && nearZero(v[2]-(*this)[2]);
    }

//////////////////////////////////////////////////////////////////////////
// Other Methods
    void Vec3f::Normalize()
    {
        double fSqr = L2Norm_Sqr();
        if(fSqr>1e-6)
            (*this) *= 1.0f/sqrt(fSqr);
    }

    double Vec3f::L2Norm_Sqr()
    {
        return _p[0]*_p[0] + _p[1]*_p[1] + _p[2]*_p[2];
    }
    
    Vec3f Vec3f::sigmoid()
    {
        Vec3f res;
        res[0] = 510.0/(1.0 + exp(-(*this)[0]/255.0)) - 255.0;
        res[1] = 510.0/(1.0 + exp(-(*this)[1]/255.0)) - 255.0;
        res[2] = 510.0/(1.0 + exp(-(*this)[2]/255.0)) - 255.0;
        return res;
    }
    
    Vec3f Vec3f::crossProd(Vec3f v)
    {
        Vec3f res;
        res[0] = (*this)[1] * v[2] - (*this)[2] * v[1];
        res[1] = (*this)[2] * v[0] - (*this)[0] * v[2];
        res[2] = (*this)[0] * v[1] - (*this)[1] * v[0];
        return res;
    }
    
    Vec3f Vec3f::prodEWise(const Vec3f v)
    {
        Vec3f res;
        res[0] = (*this)[0] * v[0];
        res[1] = (*this)[1] * v[1];
        res[2] = (*this)[2] * v[2];
        return res;
    }
}


