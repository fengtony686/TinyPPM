//
// Created by fs2001 on 2021/12/29.
//

#include "texture.h"

Texture::Texture(const std::string &f, double zoom) : zoom(zoom) {
    cv::Mat img = cv::imread(f, 1);
    this->width = img.cols;
    this->height = img.rows;
    dbg(width, height);
    this->texture = new Vec[img.cols * img.rows];
    for (int y = 0; y < img.rows; y++)
        for (int x = 0; x < img.cols; x++) {
            int i = (img.cols - x - 1) * img.rows + y;
            this->texture[i].x = double(img.at<cv::Vec3b>(y, x)[2]);
            this->texture[i].y = double(img.at<cv::Vec3b>(y, x)[1]);
            this->texture[i].z = double(img.at<cv::Vec3b>(y, x)[0]);
            this->texture[i] = this->texture[i] / 255.0;
        }
}

Vec Texture::getColorSphere(const Vec &point) const {
    double u = 0.5 + 0.5 * atan2(point.z, point.x) / M_PI;
    double v = 0.5 - asin(point.y) / M_PI;
    if (this->width == 650 || this->width == 651) {
        u = fmod(u * zoom, 1);
        v = fmod(v * zoom, 1);
    } else {
        u = fmod(u * zoom + .5, 1);
        v = fmod(v * zoom + .5, 1);
    }
    return this->texture[int((this->width - int(u * this->width) - 1) * this->height + int(v * this->height))];
}

Vec Texture::getColorBezier(const Vec &point) const {
    double u = point.x, v = point.y;
    u = fmod(u * zoom, 1);
    v = fmod(v * zoom, 1);
    int w = width, h = height;
    return texture[int(u * w) * h + int(v * h)];
}