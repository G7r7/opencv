#include <iostream>
#include <opencv2/opencv.hpp>

struct Circle
{
    size_t center_x;
    size_t center_y;
    size_t radius;
};

struct Score
{
    float score;
    size_t row;
    size_t col;
    size_t rad;
};

std::vector<Circle> hough(cv::Mat img, int circle_number)
{
    int nbRows = img.rows;
    int nbCols = img.cols;
    int nbRadius = std::sqrt(img.cols * img.cols + img.rows * img.rows);
    int gradient_threshold = 100;

    // Initializing at 0
    std::vector<std::vector<std::vector<float>>> acc(
        nbRows,
        std::vector<std::vector<float>>(
            nbCols,
            std::vector<float>(nbRadius, 0)));

    // Voting for circles
    int counter = 0;
#pragma omp parallel for
    for (size_t i = 0; i < img.rows; i++)
    {
        for (size_t j = 0; j < img.cols; j++)
        {
            // For each "border" pixel
            if ((int)(img.at<uchar>(i, j)) > gradient_threshold)
            {
                counter++;
                for (size_t r = 0; r < img.rows; r++)
                {
                    for (size_t c = 0; c < img.cols; c++)
                    {
                        int delta_x = j - c;
                        int delta_y = i - r;
                        double radius_raw = std::sqrt(delta_x * delta_x + delta_y * delta_y);
                        int radius = round(radius_raw);

                        if (radius >= 5)
                        { // We ignore circles with radius below 5 pixels
                            // We compensate the importance of bigger circle on smaller circles
                            // by dividing the vote by their circonference
                            acc[r][c][radius] += 1.f / (2 * M_PI * radius);
                        }
                    }
                }
            }
        }
    }
    std::cout << "Used border pixels : " << counter << std::endl;

    // Find local maximum
    for (size_t i = 1; i < nbRows - 4; i += 3)
    {
        for (size_t j = 1; j < nbCols - 4; j += 3)
        {
            std::vector<Score> localScores;
            for (size_t k = 1; k < nbRadius - 4; k += 3)
            {
                for (size_t index_row = i - 1; index_row < i + 2; index_row++)
                {
                    for (size_t index_col = j - 1; index_col < i + 2; index_col++)
                    {
                        for (size_t index_rad = k - 1; index_rad < k + 2; index_rad++)
                        {
                            localScores.push_back({acc[index_row][index_col][index_rad], index_row, index_col, index_rad});
                        }
                    }
                }
                std::sort(localScores.begin(), localScores.end(), [](const Score &a, const Score &b)
                          {
                    // Biggest comes first
                    return a.score > b.score; });
                if (localScores.size() > 0)
                {
                    for (size_t index_row = i - 1; index_row < i + 2; index_row++)
                    {
                        for (size_t index_col = j - 1; index_col < i + 2; index_col++)
                        {
                            for (size_t index_rad = k - 1; index_rad < k + 2; index_rad++)
                            {
                                if (!(index_row == localScores[0].row && index_col == localScores[0].col && index_rad == localScores[0].rad))
                                {
                                    acc[index_row][index_col][index_rad] = 0;
                                }
                            }
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
    std::sort(scores.begin(), scores.end(), [](const Score &a, const Score &b)
              {
        // Biggest comes first
        return a.score > b.score; });

    // We create a circle for each score
    std::vector<Circle> circles;
    int circles_limit = circle_number;
    for (auto score : scores)
    {
        if (circles_limit == 0)
        {
            break;
        }
        circles.push_back(Circle{
            score.col,
            score.row,
            score.rad});
        circles_limit -= 1;
    }

    return circles;
}

void show_usage()
{
    std::cout << "Usage: hough <image path> [<-n | --number> <number of circles>]" << std::endl;
}

int main(int argc, char **argv)
{
    int circle_number = 1;

    if (argc < 2)
    {
        show_usage();
        return -1;
    }
    if (argc == 3)
    {
        std::cout << "Missing circle number parameter" << std::endl;
        show_usage();
        return -1;
    }
    if (argc == 4)
    {
        circle_number = atoi(argv[3]);
    }

    cv::Mat image; // variable image of datatype Matrix
    image = cv::imread(argv[1]);

    // Error Handling
    if (image.empty())
    {
        std::cout << "Image file not found" << std::endl;
        return -1;
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);

    // Sobel
    cv::Mat grad_x, grad_y;
    cv::Sobel(gray, grad_x, CV_16S, 1, 0);
    cv::Sobel(gray, grad_y, CV_16S, 0, 1);

    // converting back to CV_8U
    cv::Mat abs_grad_x, abs_grad_y;
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);

    // We try to approximate the gradient by adding both directional gradients
    cv::Mat grad;
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    std::vector<Circle> circles = hough(grad, circle_number);

    cv::Mat display;
    cv::cvtColor(grad, display, cv::COLOR_GRAY2BGR);

    for (auto circle : circles)
    {
        cv::circle(image, cv::Point(circle.center_x, circle.center_y), circle.radius, cv::Scalar(0, 0, 255));
    }

    cv::imshow("Display Image", image);
    cv::waitKey(0);
    return 0;
}