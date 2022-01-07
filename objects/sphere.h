//
// Created by fs2001 on 2021/12/22.
//

#ifndef PPM_SPHERE_H
#define PPM_SPHERE_H

#include "../utils/utils.h"
#include "Object.h"
#include "../utils/texture.h"
#include "../utils/normal_texture.h"

class Sphere : public Object {
public:
    double rad;
    Texture *texture = nullptr;
    NormalTexture *normalTexture = nullptr;
    int textureType;

    Sphere(double radius_, const Vec &position_, const Vec &color, Refl_type reflectionType_,
           const std::string &textureFile = "", double zoom = 1, int textureType = 0);

    double intersect(const Ray &r, int &parameter) override;

    ~Sphere() = default;

    Vec getNormal(const Ray &r, const double &t) override;

    Vec getColor(const Vec &point) override;
};

#endif //PPM_SPHERE_H
