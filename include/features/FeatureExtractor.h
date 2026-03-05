//
// Created by PCS on 3/5/2026.
//

#ifndef HAND_SIGN_FEATUREEXTRACTOR_H
#define HAND_SIGN_FEATUREEXTRACTOR_H
#include <vector>

#include "processing/Point.h"


class FeatureExtractor {
public:
    static float fingerBend(
        const std::vector<Point>& landmarks,
        int mcp,
        int pip,
        int tip
    );

    static float thumbDistance(const std::vector<Point>& landmarks);

    static float fingerSpread(
        const std::vector<Point>& landmarks,
        int tipA,
        int tipB
    );

    static float thumbIndexDistance(const std::vector<Point>& landmarks);

    static float wristTipDistance(
        const std::vector<Point>& landmarks,
        int tip
    );

    float palmOrientationZ(const std::vector<Point>& landmarks);

    std::vector<float> extract(const std::vector<Point>& landmarks);
};


#endif //HAND_SIGN_FEATUREEXTRACTOR_H