#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

using fltType = float;

class Point2 {
   public:
    Point2() {
        e[0] = 0;
        e[1] = 0;
    }

    Point2(fltType e0, fltType e1) {
        e[0] = e0;
        e[1] = e1;
    }

    inline fltType x() const { return e[0]; }
    inline fltType y() const { return e[1]; }

    inline const Point2 &operator+() const { return *this; }
    inline Point2 operator-() const { return Point2(-e[0], -e[1]); }
    inline fltType operator[](int i) const { return e[i]; }
    inline fltType &operator[](int i) { return e[i]; }

    inline Point2 &operator+=(const Point2 &v2);
    inline Point2 &operator-=(const Point2 &v2);
    inline Point2 &operator*=(const Point2 &v2);
    inline Point2 &operator/=(const Point2 &v2);
    inline Point2 &operator*=(fltType t);
    inline Point2 &operator/=(fltType t);

    inline fltType length() const { return sqrt(e[0] * e[0] + e[1] * e[1]); }
    inline fltType squared_length() const { return e[0] * e[0] + e[1] * e[1]; }

    inline void make_unit_vector();

    fltType e[2];
};

inline std::istream &operator>>(std::istream &is, Point2 &t) {
    is >> t.e[0] >> t.e[1];
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Point2 &t) {
    os << t.e[0] << " " << t.e[1];
    return os;
}

inline Point2 operator+(const Point2 &v1, const Point2 &v2) {
    return Point2(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1]);
}

inline Point2 operator-(const Point2 &v1, const Point2 &v2) {
    return Point2(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1]);
}

inline Point2 operator*(const Point2 &v1, const Point2 &v2) {
    return Point2(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1]);
}

inline Point2 operator/(const Point2 &v1, const Point2 &v2) {
    return Point2(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1]);
}

inline Point2 operator*(fltType t, const Point2 &v) { return Point2(v.e[0] * t, v.e[1] * t); }
inline Point2 operator*(const Point2 &v, fltType t) { return Point2(v.e[0] * t, v.e[1] * t); }

inline Point2 operator/(const Point2 &v, fltType t) { return Point2(v.e[0] / t, v.e[1] / t); }

inline fltType dot(const Point2 &v1, const Point2 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1];
}

inline Point2 &Point2::operator+=(const Point2 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    return *this;
}

inline Point2 &Point2::operator-=(const Point2 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    return *this;
}

inline Point2 &Point2::operator*=(const Point2 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    return *this;
}

inline Point2 &Point2::operator/=(const Point2 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    return *this;
}

inline Point2 &Point2::operator*=(fltType t) {
    e[0] *= t;
    e[1] *= t;
    return *this;
}

inline Point2 &Point2::operator/=(fltType t) {
    fltType k = 1.0 / t;
    e[0] *= k;
    e[1] *= k;
    return *this;
}

inline Point2 unit_vector(const Point2 &v) { return v / v.length(); }

inline void Point2::make_unit_vector() {
    fltType unit_scaler = 1.0 / length();
    *this *= unit_scaler;
}