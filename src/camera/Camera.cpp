//
// Created by ADAM on 2/27/2026.
//

#include "../../include/camera/Camera.h"

bool Camera::open(int index) {
    return cap.open(index);
}

cv::Mat Camera::getFrame() {
    cv::Mat frame;
    cap >> frame;
    return frame;
}