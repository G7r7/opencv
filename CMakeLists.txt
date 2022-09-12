cmake_minimum_required (VERSION 2.8)
project (Traitement_image)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(out ${SOURCE_FILES})

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    #target_link_libraries(babyfoot -static-libgcc)
    #target_link_libraries(babyfoot -static-libstdc++)
    #file(COPY /usr/x86_64-w64-mingw32/lib/libwinpthread-1.dll DESTINATION .)
    #file(COPY /usr/lib/gcc/x86_64-w64-mingw32/10-posix/libgomp-1.dll DESTINATION .)
    #file(COPY /usr/lib/gcc/x86_64-w64-mingw32/10-posix/libgcc_s_seh-1.dll DESTINATION .)
else()
    # target_link_libraries(babyfoot dl)
    # target_link_libraries(babyfoot X11)
    # target_link_libraries(out pthread)
endif()

target_link_libraries(out ${OpenCV_LIBS} )

# file(COPY ${CMAKE_SOURCE_DIR}/src/ressources/ DESTINATION ./ressources)