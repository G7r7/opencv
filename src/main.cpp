#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat hough(cv::Mat img)
{
    int nbCols = img.cols;
    int nbRows = img.rows;
    int nbRadius = std::sqrt(img.cols * img.cols + img.rows * img.rows);
    int acc[nbRows][nbCols][nbRadius];
    int threshold = 1000;

    std::cout << nbRows;

    for (size_t i = 0; i < nbRows; i++)
    {
        for (size_t j = 0; j < nbCols; j++)
        {
            for (size_t k = 0; k < nbRadius; k++)
            {
                std::cout << i << std::endl;
                std::cout << j << std::endl;
                std::cout << k << std::endl;
                acc[i][j][k] = 0;
            }
        }
    }

    // for (size_t i = 0; i < img.rows; i++)
    // {
    //     for (size_t j = 0; j < img.cols; j++)
    //     {
    //         if (img.at<int>(i, j) > threshold)
    //         {
    //             for (size_t r = 0; r < img.rows; r++)
    //             {
    //                 for (size_t c = 0; c < img.cols; c++)
    //                 {
    //                     int delta_x = j - c;
    //                     int delta_y = i - r;
    //                     int radius = std::sqrt(delta_x * delta_x + delta_y * delta_y);

    //                     acc[r][c][radius] += 1;
    //                 }
    //             }
    //         }
    //     }
    // }

    return img;
}

int main(int argc, char **argv)
{
    cv::Mat image; // variable image of datatype Matrix
    image = cv::imread("../images/four.png");

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(image, image, cv::Size(3, 3), 0);

    cv::Mat sobel;
    cv::Sobel(image, sobel, -1, 1, 1);

    cv::Mat circles = hough(sobel);

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}