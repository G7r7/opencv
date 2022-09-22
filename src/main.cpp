#include <iostream>
#include <opencv2/opencv.hpp>

struct Circle {
    size_t center_x;
    size_t center_y;
    size_t radius;
};

struct Score {
    float score;
    size_t row;
    size_t col;
    size_t rad;
};

std::vector<Circle> hough(cv::Mat img)
{
    int nbRows = img.rows;
    int nbCols = img.cols;
    int nbRadius = std::sqrt(img.cols * img.cols + img.rows * img.rows);
    int gradient_threshold = 1000;
    
    float acc[nbRows][nbCols][nbRadius];

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
            if (img.at<int>(i, j) > gradient_threshold)
            {
                for (size_t r = 0; r < img.rows; r++)
                {
                    for (size_t c = 0; c < img.cols; c++)
                    {
                        int delta_x = j - c;
                        int delta_y = i - r;
                        double radius_raw = std::sqrt(delta_x * delta_x + delta_y * delta_y);
                        int radius = round(radius_raw);

                        if (radius >= 5) { // We ignore circles with radius below 5 pixels
                            // We compensate the importance of bigger circle on smaller circles
                            // by dividing the vote by their circonference
                            acc[r][c][radius] += 1.f/(2*M_PI*radius);
                        }
                    }
                }
            }
        }
    }

    // Find most voted circle
    std::vector<Score> scores;
    for (size_t i = 0; i < nbRows; i++)
    {
        for (size_t j = 0; j < nbCols; j++)
        {
            for (size_t k = 0; k < nbRadius; k++)
            {
                scores.push_back({acc[i][j][k], i, j, k});
            }
        }   
    }
    std::sort(scores.begin(), scores.end(), [](const Score& a, const Score& b){
        // Biggest comes first
        return a.score > b.score;
    });
    
    // We create a circle for each score
    std::vector<Circle> circles;
    int circles_limit = 4;
    for (auto score : scores)
    {
        if (circles_limit == 0) { break; }
        circles.push_back(Circle{
            score.col,
            score.row,
            score.rad
        });        
        circles_limit -= 1;
    }

    return circles;
}

int main(int argc, char **argv)
{
    cv::Mat image; // variable image of datatype Matrix
    image = cv::imread("../images/four.png");

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