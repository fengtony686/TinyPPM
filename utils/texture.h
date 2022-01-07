//
// Created by fs2001 on 2021/12/29.
//

#ifndef PPM_TEXTURE_H
#define PPM_TEXTURE_H
#include "../utils/utils.h"
#include "opencv2/opencv.hpp"


class Texture {
public:
    Vec *texture;
    double zoom;
    int width, height;
    explicit Texture(const std::string &file, double zoom);

    Vec getColorSphere(const Vec& point) const;
    Vec getColorBezier(const Vec& point) const;
};

#endif //PPM_TEXTURE_H
