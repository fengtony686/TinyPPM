//
// Created by fs2001 on 2021/12/22.
//

#include "sphere.h"

Vec Sphere::getNormal(const Ray &r, const double &t) {
    if (this->normalTexture != nullptr) {
        Vec point = r.origin + r.direction * t;
        return this->normalTexture->getNormalSphere((point-this->position).norm());
    }
    return (r.origin + r.direction * t - this->position).norm();
}


double Sphere::intersect(const Ray &r, int &parameter) {
    double b = (position - r.origin).dot(r.direction);
    double det = b * b - (position - r.origin).dot(position - r.origin) + rad * rad;
    if (det >= 0) {
        det = sqrt(det);
        if (b - det > 1e-4) {
            return b - det;
        } else if (b + det > 1e-4) {
            return b + det;
        }
    }
    return 1e20;
}

Vec Sphere::getColor(const Vec &point) {
    if (this->texture != nullptr) {
        return this->texture->getColorSphere((point-this->position).norm());
    }
    return color;
}

Sphere::Sphere(double radius_, const Vec &position_, const Vec &color, Refl_type reflectionType_,
               const std::string &file_, double zoom, int textureType) :
        rad(radius_), textureType(textureType), Object(position_, color, reflectionType_) {
    this->objt = Obj_type::SPHERE;
    if (!file_.empty() && textureType == 0) {
        this->texture = new Texture(file_, zoom);
    } else if (!file_.empty() && textureType == 1) {
        this->normalTexture = new NormalTexture(file_, zoom);
    }
}

