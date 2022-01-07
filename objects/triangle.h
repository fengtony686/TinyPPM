//
// Created by fs2001 on 2021/12/23.
//

#ifndef PPM_TRIANGLE_H
#define PPM_TRIANGLE_H

#include "../utils/utils.h"

class Triangle {
public:
    Vec a, b, c;

    Triangle(const Vec &a_, const Vec &b_, const Vec &c_) : a(a_), b(b_), c(c_) {
        this->e1 = a - b;
        this->e2 = a - c;
    };

    Vec getNormal() const {
        return (Vec() - e1).cross(Vec() - e2).norm();
    }

    double intersect(const Ray &r) {
        Vec s = a - r.origin;
        double frac = det(r.direction, e1, e2);
        double t = det(s, e1, e2) / frac;
        double beta = det(r.direction, s, e2) / frac;
        double gamma = det(r.direction, e1, s) / frac;
        if (t > 0 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1 && beta + gamma <= 1) {
            return t;
        } else {
            return 1e20;
        }
    };

    Vec getCentroid() const {
        return (a + b + c) / 3;
    }

protected:
    Vec e1;
    Vec e2;
};

#endif //PPM_TRIANGLE_H
