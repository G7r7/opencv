#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

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

std::vector<Circle> hough(cv::Mat img, int circle_number, int gradient_threshold, int min_radius)
{
    int nbRows = img.rows;
    int nbCols = img.cols;
    int nbRadius = std::sqrt(img.cols * img.cols + img.rows * img.rows);

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
        std::cout << "Voting for row: " << i << std::endl;
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

                        if (radius >= min_radius)
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

    int cube_side_size = 31;
    int cube_side_center_idx = std::floor(cube_side_size / 2);
    // Find local maximum
#pragma omp parallel for
    for (size_t i = cube_side_center_idx; i < nbRows - cube_side_size + 1; i += cube_side_size)
    {
        for (size_t j = cube_side_center_idx; j < nbCols - cube_side_size + 1; j += cube_side_size)
        {
            for (size_t k = cube_side_center_idx; k < nbRadius - cube_side_size + 1; k += cube_side_size)
            {
                std::cout << "Finding local maximum for cube centered at : (" << i << "," << j << "," << k << ")" << std::endl;
                std::vector<Score> localScores;
                for (size_t index_row = i - cube_side_center_idx; index_row < i + cube_side_center_idx + 1; index_row++)
                {
                    for (size_t index_col = j - cube_side_center_idx; index_col < j + cube_side_center_idx + 1; index_col++)
                    {
                        for (size_t index_rad = k - cube_side_center_idx; index_rad < k + cube_side_center_idx + 1; index_rad++)
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
                    for (size_t index_row = i - cube_side_center_idx; index_row < i + cube_side_center_idx + 1; index_row++)
                    {
                        for (size_t index_col = j - cube_side_center_idx; index_col < j + cube_side_center_idx + 1; index_col++)
                        {
                            for (size_t index_rad = k - cube_side_center_idx; index_rad < k + cube_side_center_idx + 1; index_rad++)
                            {
                                acc[index_row][index_col][index_rad] = 0;
                            }
                        }
                    }
                    acc[localScores[0].row][localScores[0].col][localScores[0].rad] = localScores[0].score;
                }
            }
        }
    }
    std::cout << "Fin de la recherche des maximums locaux" << std::endl;

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
    std::cout << "Usage: hough <image path> [<-n | --number> <number of circles>] [<-t | --threshold> <gradient threshold>] [<-m | --min_radius <minimum of radius>]" << std::endl;
}

int main(int argc, char **argv)
{
    int circle_number = 1;
    int gradient_threshold = 175;
    int min_radius = 5;

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
    if (argc == 5)
    {
        std::cout << "Missing gradient threshold parameter" << std::endl;
        show_usage();
        return -1;
    }
    if (argc == 6)
    {
        circle_number = atoi(argv[3]);
        gradient_threshold = atoi(argv[5]);
    }

    if (argc == 7)
    {
        std::cout << "Missing gradient min radius parameter" << std::endl;
        show_usage();
        return -1;
    }

    if (argc == 8)
    {
        circle_number = atoi(argv[3]);
        gradient_threshold = atoi(argv[5]);
        min_radius = atoi(argv[7]);
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

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Circle> circles = hough(grad, circle_number, gradient_threshold, min_radius);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Execution time: " << diff.count() << std::endl;

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