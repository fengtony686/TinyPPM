//
// Created by fs2001 on 2021/12/23.
//

#ifndef PPM_PLANE_H
#define PPM_PLANE_H

#include "Object.h"

class Plane : public Object {
public:
    Vec normal;
    Plane(const Vec& position_, const Vec& color, const Vec& normal, Refl_type reflectionType_);
    double intersect(const Ray &r, int &parameter) override;
    ~Plane() = default;
    Vec getNormal(const Ray &r, const double &t) override;
    Vec getColor(const Vec &point) override;
};


#endif //PPM_PLANE_H
