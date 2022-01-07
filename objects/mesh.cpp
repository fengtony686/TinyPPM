//
// Created by fs2001 on 2021/12/23.
//

#include "mesh.h"
#include "dbg.h"
#include <fstream>

mesh::mesh(const Vec &position_, const Vec &color_, Refl_type reflectionType, double zoom_, const std::string &file,
           const Vec &axis)
        : Sphere(0,
                 position_,
                 color_,
                 reflectionType), axis(axis),
          zoom(zoom_) {
    loadObj(file);
    initSphereAndAABB();
    std::cout << "Vertex count : " << this->allVertices.size() << std::endl;
    std::cout << "Triangles count : " << this->allTriangles.size() << std::endl;

    for (const auto &vertex: allVertices) {
        if (vertex / position > this->rad) {
            this->useSphere = false;
            std::cout << "Assert failed! " << std::endl;
            break;
        }
    }
    this->meshKDTree = new KdTree(&this->allTriangles);
    this->objt = OBJ;
}

Vec mesh::getColor(const Vec &point) {
    return this->color;
}

Vec mesh::getNormal(const Ray &r, const double &t) {
    return allTriangles[int(t)].getNormal();
}

double mesh::intersect(const Ray &r, int &parameter) {
    if (useSphere && Sphere::intersect(r, parameter) >= 1e20)
        return 1e20;
    bool intersectAABB = false;
    for (auto t: this->AABB) {
        if (t.intersect(r) < 1e20 / 2) {
            intersectAABB = true;
        }
    }
    if (!intersectAABB) {
        return 1e20;
    }
    double t = 1e20;
    static omp_lock_t lock;
    std::vector<int> indices = meshKDTree->getTriangle(r);
//    std::vector<int> indices;
//    for (int i = 0; i< allTriangles.size(); i++) {
//        indices.push_back(i);
//    }

    omp_init_lock(&lock);
#pragma omp parallel for schedule(dynamic, 12)
    for (int j = 0; j < indices.size(); j++) {
        double newT = allTriangles[indices[j]].intersect(r);
        omp_set_lock(&lock);
        if (newT < t) {
            t = newT;
            parameter = indices[j];
        }
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);
    return t > 1e-4 ? t : 1e20;
}

void mesh::initSphereAndAABB() {
    Vec maxX = allVertices[0], minX = allVertices[0], maxY = allVertices[0], minY = allVertices[0], maxZ = allVertices[0], minZ = allVertices[0];
    for (auto &vertex: allVertices) {
        if (vertex.x > maxX.x) maxX = vertex;
        if (vertex.x < minX.x) minX = vertex;
        if (vertex.y > maxY.y) maxY = vertex;
        if (vertex.y < minY.y) minY = vertex;
        if (vertex.z > maxZ.z) maxZ = vertex;
        if (vertex.z < minZ.z) minZ = vertex;
    }

    AABB.emplace_back(Vec(maxX.x, maxY.y, maxZ.z), Vec(maxX.x, maxY.y, minZ.z), Vec(maxX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(maxX.x, minY.y, minZ.z), Vec(maxX.x, maxY.y, minZ.z), Vec(maxX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(minX.x, minY.y, minZ.z), Vec(minX.x, maxY.y, minZ.z), Vec(minX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(minX.x, minY.y, minZ.z), Vec(minX.x, maxY.y, minZ.z), Vec(minX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(maxX.x, maxY.y, maxZ.z), Vec(maxX.x, maxY.y, minZ.z), Vec(minX.x, maxY.y, maxZ.z));
    AABB.emplace_back(Vec(minX.x, maxY.y, minZ.z), Vec(maxX.x, maxY.y, minZ.z), Vec(minX.x, maxY.y, maxZ.z));
    AABB.emplace_back(Vec(maxX.x, minY.y, maxZ.z), Vec(maxX.x, minY.y, minZ.z), Vec(minX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(minX.x, minY.y, minZ.z), Vec(maxX.x, minY.y, minZ.z), Vec(minX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(maxX.x, maxY.y, maxZ.z), Vec(minX.x, maxY.y, maxZ.z), Vec(maxX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(minX.x, minY.y, maxZ.z), Vec(minX.x, maxY.y, maxZ.z), Vec(maxX.x, minY.y, maxZ.z));
    AABB.emplace_back(Vec(maxX.x, maxY.y, minZ.z), Vec(minX.x, maxY.y, minZ.z), Vec(maxX.x, minY.y, minZ.z));
    AABB.emplace_back(Vec(minX.x, minY.y, minZ.z), Vec(minX.x, maxY.y, minZ.z), Vec(maxX.x, minY.y, minZ.z));

    double xDistance = maxX / minX;
    double yDistance = maxY / minY;
    double zDistance = maxZ / minZ;
    if (xDistance >= yDistance && xDistance >= zDistance) {
        this->rad = xDistance / 2.0;
        this->position = (maxX + minX) / 2.0;
    } else if (yDistance >= zDistance) {
        this->rad = yDistance / 2.0;
        this->position = (maxY + minY) / 2.0;
    } else {
        this->rad = zDistance / 2.0;
        this->position = (maxZ + minZ) / 2.0;
    }

    for (auto &vertex: allVertices) {
        Vec d = vertex - this->position;
        double dist = sqrt(d.dot(d));
        if (dist > this->rad) {
            double new_radius = (dist + this->rad) * 0.5;
            Vec shift = d * (new_radius - this->rad) / dist;
            this->rad = new_radius;
            this->position = shift + this->position;
        }
    }
}

void mesh::loadObj(const std::string &file) {
    std::ifstream input1(file);
    double bottom = 1e5;
    while (input1.getline(buffer, MAX_LENGTH)) {
        std::string line = buffer;
        if (!line.length()) continue;
        if (line[0] == 'v') {
            Vec vertex;
            int pos = 0;
            char *_;
            if (axis.x == 1) {
                vertex.x = strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.x == -1) {
                vertex.x = -strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.x == 2) {
                vertex.y = strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.x == -2) {
                vertex.y = -strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.x == 3) {
                vertex.z = strtod(readNumber(line, pos).c_str(), &_);
            } else {
                vertex.z = -strtod(readNumber(line, pos).c_str(), &_);
            }

            if (axis.y == 1) {
                vertex.x = strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.y == -1) {
                vertex.x = -strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.y == 2) {
                vertex.y = strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.y == -2) {
                vertex.y = -strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.y == 3) {
                vertex.z = strtod(readNumber(line, pos).c_str(), &_);
            } else {
                vertex.z = -strtod(readNumber(line, pos).c_str(), &_);
            }

            if (axis.z == 1) {
                vertex.x = strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.z == -1) {
                vertex.x = -strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.z == 2) {
                vertex.y = strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.z == -2) {
                vertex.y = -strtod(readNumber(line, pos).c_str(), &_);
            } else if (axis.z == 3) {
                vertex.z = strtod(readNumber(line, pos).c_str(), &_);
            } else {
                vertex.z = -strtod(readNumber(line, pos).c_str(), &_);
            }
            bottom = bottom > vertex.y ? vertex.y : bottom;
            this->allVertices.push_back(vertex);
        }
    }
    int index = 0;
    for (const auto &i: allVertices) {
        allVertices[index] = (i - Vec(0, bottom)) * zoom + position;
        index++;
    }
    std::ifstream input2(file);
    while (input2.getline(buffer, MAX_LENGTH)) {
        std::string line = buffer;
        if (!line.length()) continue;
        if (line[0] == 'f') {
            int v[3];
            char *_;
            for (int i = 0, pos = 0; i < 3; ++i) {
                v[i] = int(strtod(readNumber(line, pos).c_str(), &_) - 1);
            }
            allTriangles.emplace_back(allVertices[v[0]], allVertices[v[1]], allVertices[v[2]]);
        }
    }
}
