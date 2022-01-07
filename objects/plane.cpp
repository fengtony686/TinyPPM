//
// Created by fs2001 on 2021/12/23.
//

#include "plane.h"

Plane::Plane(const Vec& position_, const Vec& color, const Vec& normal, Refl_type reflectionType_): Object(position_, color, reflectionType_), normal(normal) {
    this->objt = PLANE;
}

Vec Plane::getNormal(const Ray &r, const double &t) {
    return normal;
}

Vec Plane::getColor(const Vec &point) {
    return color;
}

double Plane::intersect(const Ray &r, int &parameter) {
    double d = -this->position.dot(this->normal);
    double t = - (d+this->normal.dot(r.origin)) / this->normal.dot(r.direction);
    if (t > 1e-4) {
        return t;
    } else {
        return 1e20;
    }
}

