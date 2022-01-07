//
// Created by fs2001 on 2021/12/22.
//

#ifndef PPM_OBJECT_H
#define PPM_OBJECT_H
#include "../utils/utils.h"

class Object {
public:
    Vec position, color;
    Refl_type reflectionType;
    Obj_type objt = UNDEFINED;
    Object(const Vec &p_, const Vec &c_, Refl_type reflectionType_ = DIFF) :
            position(p_), color(c_), reflectionType(reflectionType_) {
    }

    virtual double intersect(const Ray &r, int &parameter) = 0;
    virtual Vec getNormal(const Ray &r, const double& t) = 0;
    __attribute__((unused)) virtual Vec getColor(const Vec &point) = 0;
};
#endif //PPM_OBJECT_H
