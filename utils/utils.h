//
// Created by fs2001 on 2021/12/22.
//

#ifndef PPM_UTILS_H
#define PPM_UTILS_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <random>
#include "dbg.h"
#include <ctime>

#define SAMPLENUM ((int)1200)
#define DEPTH ((int)15)
#define ALPHA ((double)0.7) // the alpha parameter of PPM
#define MAX_LENGTH 65536ul

double enumerateRand();
double hal(int b, int j);

inline unsigned int hash(const int ix, const int iy, const int iz, const unsigned int num_hash) {
    return (unsigned int) ((ix * 73856093) ^ (iy * 19349663) ^ (iz * 83492791)) % num_hash;
}

double clamp(double x);

int toInt(double x);

enum Refl_type {
    DIFF = 0,
    SPEC = 1,
    REFR = 2,
    MIXED = 3,
};

enum Obj_type {
    SPHERE = 0,
    OBJ = 1,
    PLANE = 2,
    BEZIER = 3,
    UNDEFINED = 4,
};

class Vec {
public:
    double x, y, z;

    explicit Vec(double x_ = 0, double y_ = 0, double z_ = 0);

    friend Vec operator/(const Vec &a, double divisor);

    Vec mul(const Vec &b) const;

    inline Vec operator+(double b) const { return Vec(x + b, y + b, z + b); }

    inline Vec operator-(double b) const { return Vec(x - b, y - b, z - b); }

    friend double operator/(const Vec &a, const Vec &b);

    double dot(const Vec &b) const;

    Vec cross(const Vec &b) const;

    Vec &norm();

    double length() const;

    void abs();

    friend std::ostream &operator<<(std::ostream &out, const Vec &a);

    double X() const { return x; }

    double Y() const { return y; }

    double Z() const { return z; }

    double getIndex(int idx) const {
        if (idx == 0) return x;
        else if (idx == 1) return y;
        else if (idx == 2) return z;
        else return 1e20;
    }

    ~Vec();
};

Vec operator+(const Vec &a, const Vec &b);

Vec operator-(const Vec &a, const Vec &b);

Vec operator*(const Vec &a, double num);


class Ray {
public:
    Vec origin, direction;

    Ray() = default;

    Ray(const Vec &_origin, Vec _direction) : origin(_origin), direction(_direction.norm()) {}
};

struct AABB {
    Vec min, max;

    inline void fit(const Vec &p) {
        max.x = max.x < p.x ? p.x : max.x;
        max.y = max.y < p.y ? p.y : max.y;
        max.z = max.z < p.z ? p.z : max.z;
        min.x = min.x > p.x ? p.x : min.x;
        min.y = min.y > p.y ? p.y : min.y;
        min.z = min.z > p.z ? p.z : min.z;
    }

    inline void reset() {
        min = Vec(1e20, 1e20, 1e20);
        max = Vec(-1e20, -1e20, -1e20);
    }
};

struct HPoint {
    Vec f, pos, normal, flux;
    double r2;
    unsigned int n;
    int pix;
};

struct List {
    HPoint *id;
    List *next;
};

List *ListAdd(HPoint *i, List *h);

double det(const Vec &a, const Vec &b, const Vec &c);

std::string readNumber(std::string line, int &pos);

#endif //PPM_UTILS_H
