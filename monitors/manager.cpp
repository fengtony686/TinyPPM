//
// Created by fs2001 on 2021/12/22.
//
#include "manager.h"

#include <utility>

void manager::addSphere(double rad_, const Vec &p_, const Vec &c_, Refl_type reflectionType, const std::string &file,
                        double zoom, int textureType) {
    objectVector[this->num] = new Sphere(rad_, p_, c_, reflectionType, file, zoom, textureType);
    this->num++;
}

void manager::addPlane(const Vec &p_, const Vec &c_, const Vec &n_, Refl_type reflectionType) {
    objectVector[this->num] = new Plane(p_, c_, n_, reflectionType);
    this->num++;
}

void manager::addMesh(const Vec &p_, const Vec &c_, Refl_type reflectionType, double zoom,
                      const std::string &file, const Vec &axis) {
    objectVector[this->num] = new mesh(p_, c_, reflectionType, zoom, file, axis);
    this->num++;
}

void
manager::addBezier(const Vec &p_, const Vec &c_, Refl_type reflectionType, Bezier2D bezier, const std::string &file_,
                   double zoom) {
    objectVector[this->num] = new Bezier3D(p_, std::move(bezier), reflectionType, c_, file_, zoom);
    this->num++;
}

bool manager::intersect(const Ray &r, double &t, int &id, int &parameter) {
    double d;
    t = 1e20;
    for (int i = this->num; i--;) {
        d = objectVector[i]->intersect(r, parameter);
        if (d < t) {
            t = d;
            id = i;
        }
    }
    return t < 1e20;
}

void manager::measure(const Ray &ray, int depth, const Vec &fl, const Vec &adj, int i) {
    double t;
    int ObjectID;
    int parameter = -1;
    depth++;
    if (!intersect(ray, t, ObjectID, parameter) || (depth >= DEPTH))return;

    int d3 = depth * 3;
    Object *obj = this->objectVector[ObjectID];
    Vec x = ray.origin + ray.direction * t, f = obj->getColor(x);
    Vec ObjectNormal;
    if (obj->objt == SPHERE || obj->objt == PLANE || obj->objt == BEZIER) {
        ObjectNormal = obj->getNormal(ray, t);
    } else {
        ObjectNormal = obj->getNormal(ray, parameter);
    }
    Vec hitNormal;
    if (ObjectNormal.dot(ray.direction) < 0 && obj->objt != SPHERE) {
        hitNormal = ObjectNormal;
    } else if (obj->objt != SPHERE) {
        hitNormal = Vec() - ObjectNormal;
    } else {
        if (ray.direction.dot(obj->position - x) > 0) {
            hitNormal = ObjectNormal;
        } else {
            hitNormal = Vec() - ObjectNormal;
        }
    }
    if (ray.direction.dot(hitNormal) >= 0) {
        return;
    }

    Refl_type nowReflectionType = obj->reflectionType;
    if (obj->reflectionType == MIXED) {
        double randNum = enumerateRand();
        if (randNum < 0.1) {
            nowReflectionType = SPEC;
        } else {
            nowReflectionType = DIFF;
        }
    }


    if (nowReflectionType == DIFF) {
        double r1 = 2. * M_PI * hal(d3 - 1, i), r2 = hal(d3 + 0, i);
        double r2s = sqrt(r2);
        Vec w = hitNormal, u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)).cross(w)).norm();
        Vec v = w.cross(u), d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();

        auto *hp = new HPoint;
        hp->f = f.mul(adj);
        hp->pos = x;
        hp->normal = ObjectNormal;
        hp->pix = int(pixel_index);
        this->hashPoints = ListAdd(hp, hashPoints);
    } else if (nowReflectionType == SPEC) {
        measure(Ray(x, ray.direction - ObjectNormal * 2.0 * ObjectNormal.dot(ray.direction)), depth, f.mul(fl),
                f.mul(adj), i);
    } else {
        Ray lr(x, ray.direction - ObjectNormal * 2.0 * ObjectNormal.dot(ray.direction));
        bool into = (ObjectNormal.dot(hitNormal) > 0.0);
        double nc = 1.0, nt = 1.5;
        double ddn = ray.direction.dot(hitNormal), cos2t, normalReflectance = pow(nt - nc, 2) / pow(nt + nc, 2), c, nnt;
        Vec tDirection;
        if (into) {
            nnt = nc / nt;
            cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
            if (cos2t < 0) return measure(lr, depth, f.mul(fl), adj, i);
            tDirection = (ray.direction * nnt - ObjectNormal * (ddn * nnt + sqrt(cos2t))).norm();
            c = 1 + ddn;
        } else {
            nnt = nt / nc;
            cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
            if (cos2t < 0) return measure(lr, depth, f.mul(fl), adj, i);
            tDirection = (ray.direction * nnt + ObjectNormal * (ddn * nnt + sqrt(cos2t))).norm();
            c = 1 - tDirection.dot(ObjectNormal);
        }
        double fresnelReflectance = normalReflectance + (1 - normalReflectance) * pow(c, 5);
        Ray rr(x, tDirection);
        Vec fa = f.mul(adj);
        measure(lr, depth, f.mul(fl), fa * fresnelReflectance, i);
        measure(rr, depth, f.mul(fl), fa * (1.0 - fresnelReflectance), i);
    }
}

void manager::trace(const Ray &ray, int depth, const Vec &fl, const Vec &adj, int i) {
    double t;
    int ObjectID;
    int parameter = -1;
    depth++;
    if (!intersect(ray, t, ObjectID, parameter) || (depth >= DEPTH))return;

    int d3 = depth * 3;
    Object *obj = this->objectVector[ObjectID];
    Vec x = ray.origin + ray.direction * t, f = obj->getColor(x);
    Vec ObjectNormal;
    if (obj->objt == SPHERE || obj->objt == PLANE || obj->objt == BEZIER) {
        ObjectNormal = obj->getNormal(ray, t);
    } else {
        ObjectNormal = obj->getNormal(ray, parameter);
    }
    Vec hitNormal;
    if (ObjectNormal.dot(ray.direction) < 0 && obj->objt != SPHERE) {
        hitNormal = ObjectNormal;
    } else if (obj->objt != SPHERE) {
        hitNormal = Vec() - ObjectNormal;
    } else {
        if (ray.direction.dot(obj->position - x) > 0) {
            hitNormal = ObjectNormal;
        } else {
            hitNormal = Vec() - ObjectNormal;
        }
    }
    if (ray.direction.dot(hitNormal) >= 0) {
        return;
    }

    Refl_type nowReflectionType = obj->reflectionType;
    if (obj->reflectionType == MIXED) {
        double randNum = enumerateRand();
        if (randNum < 0.1) {
            nowReflectionType = SPEC;
        } else {
            nowReflectionType = DIFF;
        }
    }

    double p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;

    if (nowReflectionType == DIFF) {
        double r1 = 2. * M_PI * hal(d3 - 1, i), r2 = hal(d3 + 0, i);
        double r2s = sqrt(r2);
        Vec w = hitNormal, u = ((fabs(w.x) > .1 ? Vec(0, 1) : Vec(1)).cross(w)).norm();
        Vec v = w.cross(u), d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();

        Vec hh = (x - hPointBoundingBox.min) * hash_s;
        int ix = abs(int(hh.x)), iy = abs(int(hh.y)), iz = abs(int(hh.z));
        {
            List *hp = hash_grid[hash(ix, iy, iz, num_hash)];
            while (hp != nullptr) {
                if ((hp->id->normal.dot(ObjectNormal) > 1e-3) &&
                    ((hp->id->pos - x).dot(hp->id->pos - x) <= hp->id->r2)) {
                    double g = (hp->id->n * ALPHA + ALPHA) / (hp->id->n * ALPHA + 1.0);
                    hp->id->r2 = hp->id->r2 * g;
                    hp->id->n++;
                    hp->id->flux = (hp->id->flux + hp->id->f.mul(fl) * (1. / M_PI)) * g;
                }
                hp = hp->next;
            }
        }
        if (hal(d3 + 1, i) < p) trace(Ray(x, d), depth, f.mul(fl) * (1. / p), adj, i);
    } else if (nowReflectionType == SPEC) {
        trace(Ray(x, ray.direction - ObjectNormal * 2.0 * ObjectNormal.dot(ray.direction)), depth, f.mul(fl),
              f.mul(adj), i);
    } else {
        Ray lr(x, ray.direction - ObjectNormal * 2.0 * ObjectNormal.dot(ray.direction));
        bool into = (ObjectNormal.dot(hitNormal) > 0.0);
        double nc = 1.0, nt = 1.5;
        double ddn = ray.direction.dot(hitNormal), cos2t, normalReflectance = pow(nt - nc, 2) / pow(nt + nc, 2), c, nnt;
        Vec tDirection;
        if (into) {
            nnt = nc / nt;
            cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
            if (cos2t < 0) return trace(lr, depth, f.mul(fl), adj, i);
            tDirection = (ray.direction * nnt - ObjectNormal * (ddn * nnt + sqrt(cos2t))).norm();
            c = 1 + ddn;
        } else {
            nnt = nt / nc;
            cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
            if (cos2t < 0) return trace(lr, depth, f.mul(fl), adj, i);
            tDirection = (ray.direction * nnt + ObjectNormal * (ddn * nnt + sqrt(cos2t))).norm();
            c = 1 - tDirection.dot(ObjectNormal);
        }
        double fresnelReflectance = normalReflectance + (1 - normalReflectance) * pow(c, 5);
        Ray rr(x, tDirection);
        Vec fa = f.mul(adj);
        (hal(d3 - 1, i) < fresnelReflectance) ? trace(lr, depth, f.mul(fl), fa, i) : trace(rr, depth, f.mul(fl), fa, i);
    }
}

// Generate a photon
void manager::generatePhoton(Ray *pr, Vec *flux, int i) {
    double p = 2. * M_PI * hal(0, i);
    double sinT = sin(2. * acos(sqrt(1. - hal(1, i))));
    double cosT = cos(2. * acos(sqrt(1. - hal(1, i))));
    pr->direction = Vec(cos(p) * sinT, cosT, sin(p) * sinT);
    pr->origin = Vec(50, 60, 85);
    *flux = Vec(2500, 2500, 2500) * (M_PI * 4.0);
}

void manager::buildHashGrid(const int w, const int h) {
    this->hPointBoundingBox.reset();
    List *hPointList = this->hashPoints;
    int hPointNum = 0;
    while (true) {
        hPointList->id->n = 0;
        hPointList->id->flux = Vec();
        this->hPointBoundingBox.fit(hPointList->id->pos);
        hPointList = hPointList->next;
        hPointNum++;
        if (hPointList == nullptr) {
            break;
        }
    }
    double initialRadius =
            (this->hPointBoundingBox.max - this->hPointBoundingBox.min).dot(Vec(1, 1, 1)) / (w + h) * 3.0;

    this->hPointBoundingBox.reset();
    hPointList = this->hashPoints;
    while (true) {
        hPointList->id->r2 = initialRadius * initialRadius;
        this->hPointBoundingBox.fit(hPointList->id->pos - initialRadius);
        this->hPointBoundingBox.fit(hPointList->id->pos + initialRadius);
        hPointList = hPointList->next;
        if (hPointList == nullptr) {
            break;
        }
    }

    this->hash_grid = new List *[hPointNum];
    for (unsigned int i = 0; i < hPointNum; i++) this->hash_grid[i] = nullptr;
    hPointList = this->hashPoints;
    while (true) {
        Vec BMin = ((hPointList->id->pos - initialRadius) - this->hPointBoundingBox.min) * 0.5 / initialRadius;
        BMin.abs();
        Vec BMax = ((hPointList->id->pos + initialRadius) - this->hPointBoundingBox.min) * 0.5 / initialRadius;
        BMax.abs();
        for (int iz = int(BMin.z); iz <= int(BMax.z); iz++) {
            for (int iy = int(BMin.y); iy <= int(BMax.y); iy++) {
                for (int ix = int(BMin.x); ix <= int(BMax.x); ix++) {
                    int hv = int(hash(ix, iy, iz, hPointNum));
                    this->hash_grid[hv] = ListAdd(hPointList->id, this->hash_grid[hv]);
                }
            }
        }
        hPointList = hPointList->next;
        if (hPointList == nullptr) {
            break;
        }
    }
    this->num_hash = hPointNum;
    this->hash_s = 0.5 / initialRadius;
}




