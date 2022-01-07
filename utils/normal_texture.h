//
// Created by fs2001 on 2021/12/29.
//

#ifndef PPM_NORMAL_TEXTURE_H
#define PPM_NORMAL_TEXTURE_H
#include "../utils/utils.h"
#include "opencv2/opencv.hpp"

class NormalTexture {
public:
    Vec *texture;
    double zoom;
    int width, height;
    explicit NormalTexture(const std::string &file, double zoom);

    Vec getNormalSphere(const Vec& point) const;
};
#endif //PPM_NORMAL_TEXTURE_H
