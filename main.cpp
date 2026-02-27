
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

#include "camera/Camera.h"

int main() {

    Camera camera;

    if (!camera.open(0)) {
        std::cerr << "Failed to open camera\n";
        return -1;
    }

    // some fps counter that shows up just so I know how good my camera is xD
    auto lastTime = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    double fps = 0.0;

    while (true) {

        cv::Mat frame = camera.getFrame();
        if (frame.empty())
            break;

        frameCount++;

        auto currentTime = std::chrono::high_resolution_clock::now();
        double seconds = std::chrono::duration<double>(currentTime - lastTime).count();

        if (seconds >= 1.0) {
            fps = frameCount / seconds;
            frameCount = 0;
            lastTime = currentTime;
        }

        cv::putText(frame,
                    "FPS: " + std::to_string((int)fps),
                    {10, 30},
                    cv::FONT_HERSHEY_SIMPLEX,
                    1.0,
                    {0, 255, 0},
                    2);

        cv::imshow("Webcam", frame);

        if (cv::waitKey(1) == 27)
            break;
    }

    return 0;
}
