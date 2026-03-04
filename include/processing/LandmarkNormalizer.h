//
// Created by ADAM on 2/27/2026.
//

#ifndef HAND_SIGN_LANDMARKNORMALIZER_H
#define HAND_SIGN_LANDMARKNORMALIZER_H


#pragma once
#include <vector>
#include "Point.h"

class LandmarkNormalizer {
public:
    static void normalize(std::vector<Point>& landmarks);
};


#endif //HAND_SIGN_LANDMARKNORMALIZER_H
