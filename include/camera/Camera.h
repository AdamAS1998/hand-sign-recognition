//
// Created by ADAM on 2/27/2026.
//

#ifndef HAND_SIGN_CAMERA_H
#define HAND_SIGN_CAMERA_H


#pragma once
#include <opencv2/opencv.hpp>

/**
 * camera class that opens camera, grab frame and returns it
 */
class Camera {
public:
    bool open(int index);
    cv::Mat getFrame();

private:
    cv::VideoCapture cap;
};


#endif //HAND_SIGN_CAMERA_H
