#pragma once

#include <math.h>
#include <stdlib.h>
#include <iostream>

using fltType = float;

class vec3 {
   public:
    vec3() {
        e[0] = 0;
        e[1] = 0;
        e[2] = 0;
    }

    vec3(fltType e0, fltType e1, fltType e2) {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }

    inline fltType x() const { return e[0]; }
    inline fltType y() const { return e[1]; }
    inline fltType z() const { return e[2]; }
    inline fltType r() const { return e[0]; }
    inline fltType g() const { return e[1]; }
    inline fltType b() const { return e[2]; }

    inline const vec3 &operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline fltType operator[](int i) const { return e[i]; }
    inline fltType &operator[](int i) { return e[i]; }

    inline vec3 &operator+=(const vec3 &v2);
    inline vec3 &operator-=(const vec3 &v2);
    inline vec3 &operator*=(const vec3 &v2);
    inline vec3 &operator/=(const vec3 &v2);
    inline vec3 &operator*=(fltType t);
    inline vec3 &operator/=(fltType t);

    inline fltType length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline fltType squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    inline void make_unit_vector();

    fltType e[3];
};

inline std::istream &operator>>(std::istream &is, vec3 &t) {
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const vec3 &t) {
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(fltType t, const vec3 &v) { return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t); }
inline vec3 operator*(const vec3 &v, fltType t) { return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t); }

inline vec3 operator/(const vec3 &v, fltType t) { return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t); }

inline fltType dot(const vec3 &v1, const vec3 &v2) {
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1], -(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
                v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 &vec3::operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

inline vec3 &vec3::operator-=(const vec3 &v) {
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3 &vec3::operator*=(const vec3 &v) {
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3 &vec3::operator/=(const vec3 &v) {
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3 &vec3::operator*=(fltType t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3 &vec3::operator/=(fltType t) {
    fltType k = 1.0 / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline vec3 unit_vector(const vec3 &v) { return v / v.length(); }

inline void vec3::make_unit_vector() {
    fltType unit_scaler = 1.0 / length();
    *this *= unit_scaler;
}

using point3 = vec3;