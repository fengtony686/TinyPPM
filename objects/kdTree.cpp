//
// Created by fs2001 on 2022/1/7.
//

#include "./kdTree.h"
#include "map"

AABBKDTree::AABBKDTree() = default;

AABBKDTree::AABBKDTree(const Vec &min, const Vec &max) : min(min), max(max) {}

bool AABBKDTree::isIntersect(const Ray &ray) const {
    double t1 = (min.x - ray.origin.x) / ray.direction.x;
    double t2 = (max.x - ray.origin.x) / ray.direction.x;

    double t3 = (min.y - ray.origin.y) / ray.direction.y;
    double t4 = (max.y - ray.origin.y) / ray.direction.y;

    double t5 = (min.z - ray.origin.z) / ray.direction.z;
    double t6 = (max.z - ray.origin.z) / ray.direction.z;

    double tMin = fmax(fmax(fmin(t1, t2), fmin(t3, t4)), fmin(t5, t6));
    double tMax = fmin(fmin(fmax(t1, t2), fmax(t3, t4)), fmax(t5, t6));

    if (tMax < 0 || tMin > tMax) {
        return false;
    } else {
        return true;
    }
}

bool AABBKDTree::isIntersect(const Triangle &triangle) const {
    double triangleMaxX = fmax(fmax(triangle.a.x, triangle.b.x), triangle.c.x);
    double triangleMinX = fmin(fmin(triangle.a.x, triangle.b.x), triangle.c.x);
    double triangleMaxY = fmax(fmax(triangle.a.y, triangle.b.y), triangle.c.y);
    double triangleMinY = fmin(fmin(triangle.a.y, triangle.b.y), triangle.c.y);
    double triangleMaxZ = fmax(fmax(triangle.a.z, triangle.b.z), triangle.c.z);
    double triangleMinZ = fmin(fmin(triangle.a.z, triangle.b.z), triangle.c.z);

    if (((triangleMaxX <= max.x && triangleMaxX >= min.x) || (triangleMinX <= max.x && triangleMinX >= min.x)) &&
        ((triangleMaxY <= max.y && triangleMaxY >= min.y) || (triangleMinY <= max.y && triangleMinY >= min.y)) &&
        ((triangleMaxZ <= max.z && triangleMaxZ >= min.z) || (triangleMinZ <= max.z && triangleMinZ >= min.z)))
        return true;
    if (((max.x <= triangleMaxX && max.x >= triangleMinX) || (min.x <= triangleMaxX && min.x >= triangleMinX)) &&
        ((max.y <= triangleMaxY && max.y >= triangleMinY) || (min.y <= triangleMaxY && min.y >= triangleMinY)) &&
        ((max.z <= triangleMaxZ && max.z >= triangleMinZ) || (min.z <= triangleMaxZ && min.z >= triangleMinZ)))
        return true;
    else {
        return false;
    }
}


KdTree::KdTree() = default;

KdTree::KdTree(std::vector<Triangle> *triangles) {
    this->triangles = triangles;
    std::vector<int> triangleIndex;
    triangleIndex.reserve(triangles->size());
    for (int i = 0; i < triangles->size(); i++) {
        triangleIndex.push_back(i);
    }

    Vec min = (*this->triangles)[0].a, max = (*this->triangles)[0].a;

    for (const Triangle &triangle: (*triangles)) {
        max.x = fmax(triangle.a.x, max.x);
        min.x = fmin(triangle.a.x, min.x);
        max.x = fmax(triangle.b.x, max.x);
        min.x = fmin(triangle.b.x, min.x);
        max.x = fmax(triangle.c.x, max.x);
        min.x = fmin(triangle.c.x, min.x);

        max.y = fmax(triangle.a.y, max.y);
        min.y = fmin(triangle.a.y, min.y);
        max.y = fmax(triangle.b.y, max.y);
        min.y = fmin(triangle.b.y, min.y);
        max.y = fmax(triangle.c.y, max.y);
        min.y = fmin(triangle.c.y, min.y);

        max.z = fmax(triangle.a.z, max.z);
        min.z = fmin(triangle.a.z, min.z);
        max.z = fmax(triangle.b.z, max.z);
        min.z = fmin(triangle.b.z, min.z);
        max.z = fmax(triangle.c.z, max.z);
        min.z = fmin(triangle.c.z, min.z);
    }
    int maxLevel = (int) std::fmin(log2(double(triangles->size())), MAX_LEVEL);
    int minTriangle = MIN_TRIANGLE;
    std::cout << "KdTree::KdTree() Max Level = " << maxLevel << "\n";
    std::cout << "KdTree::KdTree() Min Triangle/node = " << minTriangle << "\n";

    initNode = new Node(triangleIndex, this, AABBKDTree(min, max), 0, 0,
                        maxLevel, minTriangle);
}

std::vector<int> KdTree::getTriangle(const Ray &ray) {
    if (initNode != nullptr) {
        return initNode->getTriangle(ray);
    } else {
        std::vector<int> triangleIndex;
        triangleIndex.reserve(triangles->size());
        for (int i = 0; i < triangles->size(); i++) {
            triangleIndex.push_back(i);
        }
        return triangleIndex;
    }
}

KdTree::Node::Node() = default;

KdTree::Node::Node(const std::vector<int> &triangles, KdTree *root, const AABBKDTree &aabb, int level, int dimension,
                   int maxLevel,
                   int minTriangle) : triangles(triangles), root(root), aabb(aabb), level(level), dimension(dimension) {
    if (level < maxLevel && triangles.size() > minTriangle) {
        std::map<double, int> mapSplit;

        for (int index: triangles) {
            mapSplit[(*(root->triangles))[index].getCentroid().getIndex(dimension)] = index;
        }

        auto iterator = mapSplit.begin();
        advance(iterator, mapSplit.size() / 2);
        double splitPoint = (*(root->triangles))[iterator->second].getCentroid().getIndex(dimension);

        std::vector<int> leftIndices;
        std::vector<int> rightIndices;

        AABBKDTree aabbLeft;
        AABBKDTree aabbRight;
        Vec leftMax, rightMin;
        switch (dimension) {
            case 0:
                leftMax = Vec(splitPoint, aabb.max.y, aabb.max.z);
                rightMin = Vec(splitPoint, aabb.min.y, aabb.min.z);
                break;
            case 1:
                leftMax = Vec(aabb.max.x, splitPoint, aabb.max.z);
                rightMin = Vec(aabb.min.y, splitPoint, aabb.min.z);
                break;
            case 2:
                leftMax = Vec(aabb.max.x, aabb.max.y, splitPoint);
                rightMin = Vec(aabb.min.x, aabb.min.y, splitPoint);
                break;
            default:
                leftMax = aabb.max;
                rightMin = aabb.min;
        }
        aabbLeft = AABBKDTree(aabb.min, leftMax);
        aabbRight = AABBKDTree(rightMin, aabb.max);

        for (int index: triangles) {
            if (aabbLeft.isIntersect((*root->triangles)[index])) {
                leftIndices.push_back(index);
            }
            if (aabbRight.isIntersect((*root->triangles)[index])) {
                rightIndices.push_back(index);
            }
        }
        dimension = (dimension + 1) % 3;
        left = new Node(leftIndices, root, aabbLeft, level + 1, dimension, maxLevel, minTriangle);
        right = new Node(rightIndices, root, aabbRight, level + 1, dimension, maxLevel, minTriangle);
    } else {
        this->triangles = triangles;
        leaf = true;
    }
}

std::vector<int> KdTree::Node::getTriangle(const Ray &ray) {
    std::vector<int> triangleIndices;

    if (aabb.isIntersect(ray)) {
        if (leaf) {
            for (int index: triangles) {
                triangleIndices.push_back(index);
            }
        } else {
            std::vector<int> leftIndices;
            std::vector<int> rightIndices;
            if (left != nullptr && right != nullptr) {
                leftIndices = left->getTriangle(ray);
                rightIndices = right->getTriangle(ray);
            }
            for (int index: leftIndices) {
                triangleIndices.push_back(index);
            }
            for (int index: rightIndices) {
                triangleIndices.push_back(index);
            }
        }
    }

    return triangleIndices;
}
