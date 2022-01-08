//
// Created by fs2001 on 2022/1/7.
//

#ifndef PPM_KDTREE_H
#define PPM_KDTREE_H

#include "../utils/utils.h"
#include "./triangle.h"

#define MAX_LEVEL 20
#define MIN_TRIANGLE 25

class AABBKDTree {
public:
    AABBKDTree();

    AABBKDTree(const Vec &min, const Vec &max);

    bool isIntersect(const Ray &ray) const;

    bool isIntersect(const Triangle &face) const;

    Vec min;
    Vec max;
};

class KdTree {
public:
    KdTree();

    explicit KdTree(std::vector<Triangle> *triangles);

    std::vector<int> getTriangle(const Ray &ray);

private:

    class Node {
    public:
        Node(const std::vector<int> &triangles, KdTree *root, const AABBKDTree &aabbParent, int level, int dimension,
             int maxLevel, int minTriangle);

        Node();

        std::vector<int> getTriangle(const Ray &ray);

    private:
        int level{};
        Node *left = nullptr;
        Node *right = nullptr;
        std::vector<int> triangles;
        int dimension;
        KdTree *root{};
        AABBKDTree aabb;
        bool leaf = false;
    };

    std::vector<Triangle> *triangles{};
    Node *initNode{};
};

#endif //PPM_KDTREE_H
