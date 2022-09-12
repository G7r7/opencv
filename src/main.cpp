#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv )
{
    // Empty canvas 1707*775
    cv::Mat M(775, 1707, CV_8UC3, cv::Scalar(255,255,255));

    // Read frag file
    cv::Mat image = cv::imread("../ressources/frag_eroded/frag_eroded_2.png");

    // Rotate frag
    cv::Mat rotated_frag;
    cv::Point rotPoint(image.cols/2.0, image.rows/2.0);
    cv::Mat rotMat = cv::getRotationMatrix2D(rotPoint, -54.0116, 1);
    cv::warpAffine(image, rotated_frag, rotMat, image.size());

    // Mix
    cv::Mat mix = M(cv::Rect(640, 575, rotated_frag.cols, rotated_frag.rows));
    rotated_frag.copyTo(mix);

    // Scale for preview
    cv::Mat preview;
    cv::resize(M, preview, cv::Size(), 0.5, 0.5);

    cv::imshow("Display Image", preview);
    // cv::imwrite("out.png", M);
    cv::waitKey(0);
    return 0;
}