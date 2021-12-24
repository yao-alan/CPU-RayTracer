#ifndef VMATH
#define VMATH

#include <cmath>
#include <cstdio>

#define FOR(i, a, b) for(int i = (a); i < b; ++i)

namespace vmath 
{
    template<typename T>
    class Vec3 
    {
        public:
            Vec3(T x, T y, T z) { this->x = x; this->y = y; this->z = z; }
            Vec3(const Vec3<T> &v) { this->x = v.x; this->y = v.y; this->z = v.z; }

            // getters
            Vec3 getX() { return this->x; }
            Vec3 getY() { return this->y; }
            Vec3 getZ() { return this->z; }

            // vector operations that are func(v1, v2)
            static
            T    dot      (const Vec3 &v1, const Vec3 &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }
            static
            Vec3 cross    (const Vec3 &v1, const Vec3 &v2) { return vmath::Vec3<T>(v1.y*v2.z - v1.z*v2.y,
                                                                                   v1.z*v2.x - v1.x*v2.z,
                                                                                   v1.x*v2.y - v1.y*v2.x); }

            // vector operations specific to an object
            T    sqNorm   () { return dot(*this, *this); }
            T    norm     () { return std::sqrt(this->sqNorm()); }
            Vec3 normalize() { return *this / this->norm(); }

            // operator overloading
            Vec3 operator+(const Vec3 &v) { return Vec3<T>(this->x+v.x, this->y+v.y, this->z+v.z); }
            Vec3 operator-(const vmath::Vec3<T> &v) { return vmath::Vec3<T>(this->x-v.x, this->y-v.y, this->z-v.z); }
            Vec3 operator*(const vmath::Vec3<T> &v) { return vmath::Vec3<T>(this->x*v.x, this->y*v.y, this->z*v.z); }
            Vec3 operator*(T lambda) { return vmath::Vec3<T>(lambda*this->x, lambda*this->y, lambda*this->z); }
            Vec3 operator/(T lambda) { return (1/lambda) * *this; }

            Vec3 &operator+=(const Vec3 &v) { this->x += v.x; this->y += v.y; this->z += v.z; return *this; }
            Vec3 &operator-=(const Vec3 &v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; return *this; }
            Vec3 &operator*=(const vmath::Vec3<T> &v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; return *this; }
            Vec3 &operator*=(const T lambda) { this->x *= lambda; this->y *= lambda; this->z *= lambda; return *this; }
            Vec3 &operator/=(const T lambda) { return *this *= 1/lambda; }
        private:
            T x, y, z;
    };

    template<typename T>
    class Mat4 
    {
        public:
            Mat4(T mat[4][4]) { FOR(i, 0, 4) { FOR(j, 0, 4) this->mat[i][j] = mat[i][j]; } }
            Mat4(const Mat4& m) { FOR(i, 0, 4) { FOR(j, 0, 4) this->mat[i][j] = m.mat[i][j]; } }
            // operator overloading
            Mat4 &operator+(const Mat4 &m) {}

            Mat4 &operator+=(const Mat4 &m) { FOR(i, 0, 4) { FOR(j, 0, 4) this->mat[i][j] += m.mat[i][j]; } return *this; }
            Mat4 &operator-=(const Mat4 &m) { FOR(i, 0, 4) { FOR(j, 0, 4) this->mat[i][j] -= m.mat[i][j]; } return *this; }
            Mat4 &operator*=(const Mat4 &m) { FOR(i, 0, 4) { FOR(j, 0, 4) { FOR(k, 0, 4) { this->mat[i][j] += this->mat[i][k]*m.mat[k][j]; } } } return *this; }
            Mat4 &operator*=(const T lambda) { FOR(i, 0, 4) { FOR(j, 0, 4) this->mat[i][j] *= lambda; } return *this; }
            Mat4 &operator/=(const T lambda) { return *this *= 1/lambda; }
        private:
            T mat[4][4];
    };
};

#endif