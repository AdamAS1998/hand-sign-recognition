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

    // Remove scale so hand size doesn't matter
    float scale = landmarks[12].length();

    // Scale all points (should play more with scaler threshold )
    // note that small values should be ignored cuz it indicates hands partially
    // lost, detection glitch, middle tip misdetected near wrist or frame noise.
    // maybe look to add upper bound as well to avoid weird behavior ?
    if (scale < 0.05f || scale > 1.0f) {
        return;  // skip this frame
    }
    // Good to scale
    for (auto& p : landmarks) {
        p = p / scale;
    }

}