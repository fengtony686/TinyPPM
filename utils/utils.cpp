//
// Created by fs2001 on 2021/12/22.
//

#include "utils.h"

std::default_random_engine re(time(nullptr));

double hal(const int b, int j) {
    int primes[61] = {
            2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
            83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
            191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283
    };
    const int p = primes[b];
    double h = 0.0, f = 1.0 / (double) p, fct = f;
    while (j > 0) {
        if (j % p != 0) {
            h += (p - j % p) * fct;
        }
        j /= p;
        fct *= f;
    }
    return h;
}

double clamp(double x) { return x < 0 ? 0 : x > 1 ? 1 : x; }

int toInt(double x) { return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }

Vec::Vec(double x_, double y_, double z_) :
        x(x_), y(y_), z(z_) {
}

Vec operator+(const Vec &a, const Vec &b) {
    return Vec(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec operator-(const Vec &a, const Vec &b) {
    return Vec(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vec operator*(const Vec &a, double num) {
    return Vec(a.x * num, a.y * num, a.z * num);
}

double operator/(const Vec &a, const Vec &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}

Vec operator/(const Vec &a, double divisor) {
    double x = a.X() / divisor;
    double y = a.Y() / divisor;
    double z = a.Z() / divisor;
    return Vec(x, y, z);
}

Vec Vec::mul(const Vec &b) const {
    return Vec(this->x * b.x, this->y * b.y, this->z * b.z);
}

double Vec::dot(const Vec &b) const {
    return this->x * b.x + this->y * b.y + this->z * b.z;
}

Vec Vec::cross(const Vec &b) const {
    return Vec(this->y * b.z - this->z * b.y,
               this->z * b.x - this->x * b.z,
               this->x * b.y - this->y * b.x);
}

Vec &Vec::norm() {
    return *this = *this * (1 / sqrt(x * x + y * y + z * z));
}

std::ostream &operator<<(std::ostream &out, const Vec &a) {
    out << a.x << " " << a.y << " " << a.z;
    return out;
}

double Vec::length() const {
    return sqrt(x * x + y * y + z * z);
}

void Vec::abs() {
    this->x = x >= 0 ? x : -x;
    this->y = y >= 0 ? y : -y;
    this->z = z >= 0 ? z : -z;
}

Vec::~Vec() = default;


List *ListAdd(HPoint *i, List *h) {
    List *p = new List;
    p->id = i;
    p->next = h;
    return p;
}

double det(const Vec &a, const Vec &b, const Vec &c) {
    return a.x * b.y * c.z + a.y * b.z * c.x + a.z * b.x * c.y - a.x * b.z * c.y - a.y * b.x * c.z - a.z * b.y * c.x;
}

std::string readNumber(std::string line, int &pos) {
    auto is_number = [](char c) -> bool {
        return ('0' <= c && c <= '9') || c == '.' || c == '-' || c == 'e';
    };
    int start = pos;
    while (start < line.length() && !is_number(line[start])) ++start;
    pos = start + 1;
    while (pos < line.length() && is_number(line[pos])) ++pos;
    return line.substr(start, pos - start);
}

double enumerateRand() {
    return (double) (re() % 100000) / ((double) 100000);
}