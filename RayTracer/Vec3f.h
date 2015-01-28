#pragma once
#include <cassert>
#include <iostream>
namespace SimpleOBJ
{
    class Vec3f
    {
    public:

        //Constructors
        Vec3f();
        Vec3f(double x, double y, double z);
        Vec3f(const Vec3f& v);
        //Deconstructor
        virtual ~Vec3f();
    public:
        //Operators

        //Operator []
        inline double& operator [](int index)
        {
            assert(index>=0&&index<3);
            return _p[index];
        }
        inline const double& operator [](int index) const
        {
            assert(index>=0&&index<3);
            return _p[index];
        }
        
        //Operator =
        Vec3f& operator = (const Vec3f& v);

        //Operators +=,-=, *=, /=
        Vec3f& operator +=(const Vec3f& v);
        Vec3f& operator +=(double f);
        void operator -=(const Vec3f& v);
        void operator -=(double f);
        void operator *=(const Vec3f& v);
        void operator *=(double f);
        void operator /=(const Vec3f& v);
        void operator /=(double f);
        
        bool operator ==(const Vec3f & v);
        bool near(const Vec3f & v) const;
        //Operators +,-.*,/
        Vec3f operator +(const Vec3f&v) const;
        Vec3f operator +(double f) const;
        Vec3f operator -(const Vec3f&v) const;
        Vec3f operator -(double f) const;
        double operator *(const Vec3f&v) const;
        Vec3f operator *(double f) const;
        Vec3f operator /(const Vec3f&v) const;
        Vec3f operator /(double f) const;

        Vec3f operator -() const;

    public:
        Vec3f sigmoid();
        Vec3f prodEWise(const Vec3f v);
        void Normalize();
        double L2Norm_Sqr();
        Vec3f crossProd(Vec3f v);
        friend std::ostream & operator<<(std::ostream & os, const Vec3f & v)
        {
            os << "[" << v[0] << "," << v[1] << "," << v[2] << "]";
            return os;
        }
    public:
        union
        {
            struct
            { double _p[3]; };
            struct
            { double x,y,z; };
            struct
            { double r,g,b; };
        };
        enum {_len = 3};   
        
    };
}

 