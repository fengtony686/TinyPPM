//
// Created by fs2001 on 2021/12/23.
//

#ifndef PPM_MESH_H
#define PPM_MESH_H

#include "sphere.h"
#include "triangle.h"
#include "kdTree.h"
#include "omp.h"

class mesh : public Sphere {
public:
    bool useSphere = true;
    double zoom;
    std::vector<Vec> allVertices;
    std::vector<Triangle> allTriangles;
    std::vector<Triangle> AABB;
    Vec axis;
    KdTree * meshKDTree;
    char buffer[MAX_LENGTH]{};

    mesh(const Vec& position_, const Vec& color_, Refl_type reflectionType, double zoom_ = 10, const std::string& file = "", const Vec& axis = Vec(1,2,3));

    ~mesh() = default;

    Vec getColor(const Vec &point) override;

    Vec getNormal(const Ray&r , const double &t) override;

    double intersect(const Ray &r, int &parameter) override;

    void loadObj(const std::string& file = "");

    void initSphereAndAABB();

};

#endif //PPM_MESH_H
