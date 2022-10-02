cmake_minimum_required (VERSION 2.8)
project (Traitement_image)

add_executable(out "src/main.cpp")
add_executable(evaluator "src/evaluator.cpp")

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_libraries(out ${OpenCV_LIBS} )