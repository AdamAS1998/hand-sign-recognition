//
// Created by PCS on 3/5/2026.
//

#include "../../include/features/FeatureExtractor.h"

float FeatureExtractor::fingerBend(
        const std::vector<Point>& landmarks,
        int mcp,
        int pip,
        int tip)
{
    Point v1 = landmarks[pip] - landmarks[mcp];
    Point v2 = landmarks[tip] - landmarks[pip];

    float dot = v1.dot(v2);
    float len = v1.length() * v2.length();

    if (len == 0)
        return 0;

    return dot / len; // cos(angle)
}

float FeatureExtractor::thumbDistance(const std::vector<Point>& landmarks)
{
    Point palmCenter = (landmarks[5] + landmarks[17]) / 2.0f;

    Point diff = landmarks[4] - palmCenter;

    return diff.length();
}

float FeatureExtractor::fingerSpread(
    const std::vector<Point>& landmarks,
    int tipA,
    int tipB)
{
    return (landmarks[tipA] - landmarks[tipB]).length();
}

float FeatureExtractor::thumbIndexDistance(
    const std::vector<Point>& landmarks)
{
    return (landmarks[4] - landmarks[8]).length();
}

float FeatureExtractor::wristTipDistance(
    const std::vector<Point>& landmarks,
    int tip)
{
    return (landmarks[tip] - landmarks[0]).length();
}



std::vector<float> FeatureExtractor::extract(
    const std::vector<Point>& landmarks)
{
    std::vector<float> features;

    // Thumb distance
    features.push_back(thumbDistance(landmarks));

    // Thumb to Index distance
    features.push_back((landmarks[4] - landmarks[8]).length());

    // Finger bends (MCP, PIP, TIP)
    features.push_back(fingerBend(landmarks, 5, 6, 8));   // index
    features.push_back(fingerBend(landmarks, 9, 10, 12)); // middle
    features.push_back(fingerBend(landmarks, 13, 14, 16));// ring
    features.push_back(fingerBend(landmarks, 17, 18, 20));// pinky
    features.push_back(fingerBend(landmarks, 2, 3, 4)); // thumb

    // Finger spreads
    features.push_back((landmarks[8]  - landmarks[12]).length()); // IM
    features.push_back((landmarks[12] - landmarks[16]).length()); // MR
    features.push_back((landmarks[16] - landmarks[20]).length()); // RP

    // Wrist distances
    features.push_back((landmarks[8]  - landmarks[0]).length()); // index-wrist
    features.push_back((landmarks[12] - landmarks[0]).length()); // middle-wrist

    Point normal = (landmarks[5] - landmarks[0]).cross(landmarks[17] - landmarks[0]);

    // Palm orientations
    features.push_back(normal.z);
    features.push_back(normal.y);
    features.push_back(normal.x);


    return features;
}