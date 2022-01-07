//
// Created by fs2001 on 2021/12/31.
//

#ifndef PPM_BEZIEROBJECT_H
#define PPM_BEZIEROBJECT_H

#include "Object.h"
#include <utility>
#include "bezier2D.h"
#include "../utils/texture.h"

class Bezier3D : public Object {
public:
    Bezier2D curve;
    Texture *texture = nullptr;

    Bezier3D(const Vec &pos_, Bezier2D curve_, Refl_type reflectionType, const Vec &color_,
             const std::string &textureFile = "", double zoom = 1) : curve(std::move(curve_)),
                                                                     Object(pos_, color_,
                                                                            reflectionType) {
        this->objt = BEZIER;
        if (!textureFile.empty()) {
            this->texture = new Texture(textureFile, zoom);
        }
    }

    ~Bezier3D() = default;

    Vec getColor(const Vec &point)
    override {
        if (texture != nullptr) {
            double t = curve.solveT(point.y - position.y);
            double u = atan2(point.z - position.z, point.x - position.x);
            if (u < 0)
                u += 2 * M_PI;
            return texture->getColorBezier(Vec(u / 2 / M_PI + .5, t));
        }
        return this->color;
    };

    Vec getNormal(const Ray &r, const double &t)
    override {
        Vec intersection = r.origin + r.direction * t;
        double tSolved = curve.solveT(intersection.y - position.y);
        double distance = intersection / (position + Vec(0, intersection.y - position.y));
        Vec dir = curve.getDerivation(tSolved);
        return Vec(-intersection.z + position.z, 0, intersection.x - position.x).cross(
                Vec(intersection.x - position.x, dir.y / dir.x * distance, intersection.z - position.z)).norm();
    };

    double intersect(const Ray &ray, int &parameter)
    override {
        double final_dis = 1e20;
        if (std::abs(ray.direction.y) < 5e-4) {
            double t = curve.solveT(ray.origin.y - position.y);
            if (int(t) == -1)
                return 1e20;
            Vec loc = curve.computePosition(t);
            double b = (Vec(position.x, position.y + loc.y, position.z) - ray.origin).dot(ray.direction);
            double det = b * b - (Vec(position.x, position.y + loc.y, position.z) - ray.origin).dot(
                    Vec(position.x, position.y + loc.y, position.z) - ray.origin) + loc.x * loc.x;
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
        double a, b, c, t1, t2, t3, t4;
        t1 = ray.direction.x / ray.direction.y;
        t2 = ray.origin.x - position.x - t1 * ray.origin.y;
        t3 = ray.direction.z / ray.direction.y;
        t4 = ray.origin.z - position.z - t3 * ray.origin.y;
        a = t1 * t1 + t3 * t3;
        b = -position.y - (t1 * t2 + t3 * t4) / (t1 * t1 + t3 * t3);
        c = pow(t2 * t3 - t1 * t4, 2) / (t1 * t1 + t3 * t3);
        if (c > pow(curve.maxX, 2) || (b < 0 && b * b * a + c > pow(curve.maxX, 2)) ||
            (b > curve.height && pow(curve.height - b, 2) * a + c > pow(curve.maxX, 2)))
            return 1e20;
        for (int count = 0; count <= curve.num; ++count) {
            double segmentT0 = curve.data[count].t0, segmentT1 = curve.data[count].t1;
            {
                double p1 = newton((segmentT0 * 2 + segmentT1) / 3, a, b, c, segmentT0, segmentT1);
                double p2 = newton((segmentT0 + segmentT1 * 2) / 3, a, b, c, segmentT0, segmentT1);
                final_dis = int(p1) == -1 ? final_dis : std::min(final_dis,
                                                                 (position.y + curve.computePosition(p1).y -
                                                                  ray.origin.y) / ray.direction.y);
                final_dis = int(p2) == -1 ? final_dis : std::min(final_dis,
                                                                 (position.y + curve.computePosition(p2).y -
                                                                  ray.origin.y) / ray.direction.y);
            }
        }
        return final_dis > 1e-4 ? final_dis : 1e20;
    };

    double newton(double t, double a, double b, double c, double low, double upp) {
        double ft, dft, sq;
        Vec loc, dir;
        for (int i = 10; i--;) {
            t = t < low ? low : t > upp ? upp : t;
            loc = curve.computePosition(t), dir = curve.getDerivation(t);
            sq = sqrt(a * pow(loc.y - b, 2) + c);
            ft = loc.x - sq;
            if (std::abs(ft) < 1e-6)
                return t;
            dft = dir.x - a * (loc.y - b) * dir.y / sq;
            t -= ft / dft;
        }
        return -1;
    }
};

#endif //PPM_BEZIEROBJECT_H
