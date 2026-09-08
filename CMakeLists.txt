cmake_minimum_required(VERSION 3.20)

project(vanttecUSV VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenCV REQUIRED)

add_executable(vanttecUSV
    src/main.cpp
    src/Vision.cpp
)

target_include_directories(vanttecUSV
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(vanttecUSV
    PRIVATE
        ${OpenCV_LIBS}
)

add_executable(calibrarColor
    src/calibrarColor.cpp
)

target_include_directories(calibrarColor
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(calibrarColor
    PRIVATE
        ${OpenCV_LIBS}
)