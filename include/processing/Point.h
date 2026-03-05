//
// Created by PCS on 3/4/2026.
//

#ifndef HAND_SIGN_POINT_H
#define HAND_SIGN_POINT_H


#pragma once
#include <cmath>

struct Point {
    float x;
    float y;
    float z;

    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Point operator/(float scalar) const {
        return {x / scalar, y / scalar, z / scalar};
    }

    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    float dot(const Point& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Point operator+(const Point& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Point cross(const Point& other) const {
        return {
            y*other.z - z*other.y,
            z*other.x - x*other.z,
            x*other.y - y*other.x
        };
    }
};


#endif //HAND_SIGN_POINT_H