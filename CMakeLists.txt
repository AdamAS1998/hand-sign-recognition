cmake_minimum_required(VERSION 3.27)
project(Hand_Sign)

set(CMAKE_CXX_STANDARD 23)

add_executable(Hand_Sign main.cpp
        include/camera/Camera.h
        src/camera/Camera.cpp
        include/camera/Camera.h
        src/calibration/CalibrationManager.cpp
        include/calibration/CalibrationManager.h
        src/machineLearning/Classifier.cpp
        include/machineLearning/Classifier.h
        src/machineLearning/KNNClassifier.cpp
        include/machineLearning/KNNClassifier.h
        src/machineLearning/SVMClassifier.cpp
        include/machineLearning/SVMClassifier.h
        src/processing/LandmarkNormalizer.cpp
        include/processing/LandmarkNormalizer.h
        src/text/TextBuffer.cpp
        include/text/TextBuffer.h
        src/tracking/HandTracker.cpp
        include/tracking/HandTracker.h
        src/tracking/Landmark.cpp
        include/tracking/Landmark.h
)
