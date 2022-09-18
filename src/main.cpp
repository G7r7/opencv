#include <iostream>
#include <opencv2/opencv.hpp>

struct Circle {
    size_t center_x;
    size_t center_y;
    size_t radius;
};

std::vector<Circle> hough(cv::Mat img)
{
    int nbRows = img.rows;
    int nbCols = img.cols;
    int nbRadius = std::sqrt(img.cols * img.cols + img.rows * img.rows);
    int acc[nbRows][nbCols][nbRadius];
    int threshold = 1000;

    // Initialization
    for (size_t i = 0; i < nbRows; i++)
    {
        for (size_t j = 0; j < nbCols; j++)
        {
            for (size_t k = 0; k < nbRadius; k++)
            {
                acc[i][j][k] = 0;
            }
        }
    }

    // Voting for circles
    for (size_t i = 0; i < img.rows; i++)
    {
        for (size_t j = 0; j < img.cols; j++)
        {
            // For each "border" pixel 
            if (img.at<int>(i, j) > threshold)
            {
                for (size_t r = 0; r < img.rows; r++)
                {
                    for (size_t c = 0; c < img.cols; c++)
                    {
                        int delta_x = j - c;
                        int delta_y = i - r;
                        int radius = std::sqrt(delta_x * delta_x + delta_y * delta_y);

                        acc[r][c][radius] += 1;
                    }
                }
            }
        }
    }

    // Find most voted circle
    size_t maximum_score = 0;
    size_t max_row, max_col, max_rad = 0;
    for (size_t i = 0; i < nbRows; i++)
    {
        for (size_t j = 0; j < nbCols; j++)
        {
            for (size_t k = 0; k < nbRadius; k++)
            {
                if (acc[i][j][k] > maximum_score)
                {
                    maximum_score = acc[i][j][k];
                    max_row = i;
                    max_col = j;
                    max_rad = k;
                }
                
            }
            
        }   
    }

    std::cout << maximum_score << std::endl;
    std::cout << max_row << std::endl;
    std::cout << max_col << std::endl;
    std::cout << max_rad << std::endl;
    
    std::vector<Circle> circles;
    circles.push_back(Circle{
        max_row,
        max_col,
        max_rad
    });

    return circles;
}

int main(int argc, char **argv)
{
    cv::Mat image; // variable image of datatype Matrix
    image = cv::imread("../images/fourn.png");

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);

    cv::Mat sobel;
    cv::Sobel(gray, sobel, -1, 1, 1);

    std::vector<Circle> circles = hough(sobel);

    for (auto circle : circles) {
        cv::circle(image, cv::Point(circle.center_x, circle.center_y), circle.radius, cv::Scalar(0, 0, 255));
    }

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}