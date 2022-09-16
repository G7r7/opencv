#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    cv::Mat image; // variable image of datatype Matrix
    image = cv::imread("../images/four.png");

    cv::GaussianBlur(image, image, cv::Size(3, 3), 0);

    cv::Mat sobel;
    cv::Sobel(image, sobel, -1, 1, 1);

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}