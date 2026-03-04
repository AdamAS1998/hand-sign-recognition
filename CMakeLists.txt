cmake_minimum_required(VERSION 3.27)
project(Hand_Sign)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV REQUIRED)

add_executable(Hand_Sign
        main.cpp
        src/calibration/CalibrationManager.cpp
        src/machineLearning/Classifier.cpp
        src/machineLearning/KNNClassifier.cpp
        src/machineLearning/SVMClassifier.cpp
        src/processing/LandmarkNormalizer.cpp
        src/text/TextBuffer.cpp
        src/tracking/HandTracker.cpp
        src/tracking/Landmark.cpp
        src/network/TCPServer.cpp
        include/network/TCPServer.h
        src/processing/Point.cpp
        include/processing/Point.h
        include/json/json.hpp
)

target_include_directories(Hand_Sign PRIVATE include)

target_link_libraries(Hand_Sign PRIVATE ${OpenCV_LIBS})