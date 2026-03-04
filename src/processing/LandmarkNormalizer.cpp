//
// Created by ADAM on 2/27/2026.
//

#include "../../include/processing/LandmarkNormalizer.h"
#include "../../include/processing/Point.h"


void LandmarkNormalizer::normalize(std::vector<Point>& landmarks) {

    if (landmarks.size() != 21) return;

    // Remove translation (wrist origin)
    Point wrist = landmarks[0];

    for (auto& p : landmarks) {
        p = p - wrist;
    }

    // Remove scale
    float scale = landmarks[12].length();

    if (scale > 0.0001f) {
        for (auto& p : landmarks) {
            p = p / scale;
        }
    }
}