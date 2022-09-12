#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv )
{
    cv::Mat image;  // variable image of datatype Matrix
    image = cv::imread("../tux.jpg");

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}