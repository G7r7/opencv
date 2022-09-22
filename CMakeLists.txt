cmake_minimum_required (VERSION 2.8)
project (Traitement_image)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(out ${SOURCE_FILES})

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")

else()

endif()

target_link_libraries(out ${OpenCV_LIBS} )

find_package(OpenMP)
if (OPENMP_FOUND)
    target_link_libraries(out OpenMP::OpenMP_CXX)
endif()