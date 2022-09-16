#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char **argv)
{
    cv::Mat image; // variable image of datatype Matrix
    cv::Mat blurred;
    image = cv::imread("../images/fourn.png");

    cv::GaussianBlur(image, image, cv::Size(21, 21), 0);

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}