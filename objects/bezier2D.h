//
// Created by fs2001 on 2021/12/31.
//

#ifndef PPM_BEZIER2D_H
#define PPM_BEZIER2D_H

#include "../utils/utils.h"
#include "vector"

class Bezier2D {
public:
    double maxX = 0, height, r, num;
    std::vector<Vec> coefficients;
    struct D {
        double t0, t1, width = 0;
    } data[20]{};

    Bezier2D(std::vector<double> &px, std::vector<double> py, int num_, double r_) : num(num_),
                                                                                     r(r_) {
        coefficients.reserve(int(num));
        int n = int(num) - 1;

        // Preprocessing and compute the coefficients in Q(f, t)
        for (int i = 0; i <= n; ++i) {
            coefficients.emplace_back(Vec(px[0], py[0]));
            for (int j = 0; j <= n - i; ++j) {
                px[j] = px[j + 1] - px[j];
                py[j] = py[j + 1] - py[j];
            }
        }
        double coefficient = 1;
        for (int i = 0; i <= n; ++i) {
            coefficients[i] = coefficients[i] * coefficient;
            coefficient *= double(n - i) / (i + 1);
        }
        this->height = computePosition(1).y;

        double c;
        for (int count = 0; count <= num - 1; ++count) {
            c = count / (num - 1);
            data[count].t0 = std::max(0., c - r);
            data[count].t1 = std::min(1., c + r);
            double t = data[count].t0;
            while (t <= data[count].t1) {
                Vec pos = computePosition(t);
                data[count].width = std::max(pos.x, data[count].width);
                t += 1e-5;
            }
            maxX = std::max(data[count].width, maxX);
            data[count].width += 1e-6;
        }
        this->maxX += 1e-6;
    }

    double solveT(double yc) {
        double t = .5, ft, dft;
        for (int i = 10; i--;) {
            t = clamp(t);
            ft = computePosition(t).y - yc;
            if (std::abs(ft) < 1e-6)
                return t;
            dft = getDerivation(t).y;
            t -= ft / dft;
        }
        return -1;
    }

    Vec computePosition(double t) {
        Vec position = Vec();
        double tPow = 1;
        for (int i = 0; i <= num - 1; ++i) {
            position = position + coefficients[i] * tPow;
            tPow *= t;
        }
        return position;
    }

    Vec getDerivation(double t) {
        Vec position = Vec();
        double tPow = 1;
        for (int i = 1; i <= num - 1; ++i) {
            position = position + coefficients[i] * tPow * i;
            tPow *= t;
        }
        return position;
    }
};


#endif //PPM_BEZIER2D_H
