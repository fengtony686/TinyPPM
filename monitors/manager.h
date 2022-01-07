//
// Created by fs2001 on 2021/12/22.
//

#ifndef PPM_MANAGER_H
#define PPM_MANAGER_H

#include <vector>
#include "../objects/sphere.h"
#include "../objects/plane.h"
#include "../objects/mesh.h"
#include "../objects/bezierObject.h"

class manager {
public:
    unsigned int num_hash{}, pixel_index{}, num_photon{};
    double hash_s{};
    List **hash_grid{};
    List *hashPoints = nullptr;
    AABB hPointBoundingBox;

    std::vector<Object *> objectVector;
//    Camera *cam;
    int num = 0;

    manager() {
        this->objectVector.assign(100, nullptr);
    }

    void addPlane(const Vec &p_, const Vec &c_, const Vec &n_, Refl_type reflectionType);

    void addSphere(double rad_, const Vec &p_, const Vec &c_, Refl_type reflectionType, const std::string &file_ = "",
                   double zoom = 1, int textureType = 0);

    void addMesh(const Vec &p_, const Vec &c_, Refl_type reflectionType, double zoom, const std::string &file = "", const Vec& axis = Vec(1,2,3));

    void
    addBezier(const Vec &p_, const Vec &c_, Refl_type reflectionType, Bezier2D bezier, const std::string &file_ = "",
              double zoom = 1);

    bool intersect(const Ray &r, double &t, int &id, int &parameter);

    void trace(const Ray &r, int depth, const Vec &fl, const Vec &adj, int i);

    void measure(const Ray &r, int depth, const Vec &fl, const Vec &adj, int i);

    void buildHashGrid(int w, int h);

    static void generatePhoton(Ray *pr, Vec *f, int i);
};

#endif //PPM_MANAGER_H
