cmake_minimum_required(VERSION 3.27)
project(Hand_Sign)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenCV REQUIRED)

# Recognition program
add_executable(recognition
        recognition/main.cpp
        src/calibration/CalibrationManager.cpp
        src/machineLearning/Classifier.cpp
        src/machineLearning/KNNClassifier.cpp
        src/machineLearning/SVMClassifier.cpp
        src/processing/LandmarkNormalizer.cpp
        src/text/TextBuffer.cpp
        src/tracking/HandTracker.cpp
        src/tracking/Landmark.cpp
        src/network/TCPServer.cpp
        src/processing/Point.cpp
        src/features/FeatureExtractor.cpp
)

target_include_directories(recognition PRIVATE include)
target_link_libraries(recognition PRIVATE ${OpenCV_LIBS})


# Dataset recorder
add_executable(sample_gather
        sample_gather/main.cpp
        src/network/TCPServer.cpp
        src/features/FeatureExtractor.cpp
        src/processing/LandmarkNormalizer.cpp
        src/processing/Point.cpp
)

target_include_directories(sample_gather PRIVATE include)
target_link_libraries(sample_gather PRIVATE ${OpenCV_LIBS})