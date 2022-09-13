#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>

struct fragment_solution
{
    int index;
    int heigth;
    int width;
    float angle;
};

int main(int argc, char **argv)
{
    // Empty canvas 1707*775
    cv::Mat M(1500, 2500, CV_8UC4, cv::Scalar(255, 255, 255, 255));

    // Read frag file

    std::ifstream solutions("../ressources/fragments.txt");
    char line[1000];

    while (solutions.getline(line, 1000))
    {
        fragment_solution fragment_solutions;
        std::string s(line);
        std::string delimiter = " ";
        size_t pos = 0;
        std::vector<std::string> tokens;

        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            tokens.push_back(s.substr(0, pos));
            s.erase(0, pos + delimiter.length());
        }
        tokens.push_back(s);

        fragment_solutions = {
            std::stoi(tokens[0]),
            std::stoi(tokens[1]),
            std::stoi(tokens[2]),
            std::stof(tokens[3])};

        // Load image
        cv::Mat image = cv::imread(std::string("../ressources/frag_eroded/frag_eroded_") + std::to_string(fragment_solutions.index) + std::string(".png"), cv::IMREAD_UNCHANGED);

        // Rotate frag
        cv::Mat rotated_frag;
        cv::Point rotPoint(image.cols / 2.0, image.rows / 2.0);
        cv::Mat rotMat = cv::getRotationMatrix2D(rotPoint, fragment_solutions.angle, 1);
        cv::warpAffine(image, rotated_frag, rotMat, image.size());

        // Mask
        std::vector<cv::Mat> channels(4);
        cv::split(rotated_frag, channels);

        // Mix
        cv::Mat mix = M(cv::Rect(fragment_solutions.heigth, fragment_solutions.width, rotated_frag.cols, rotated_frag.rows));

        cv::copyTo(rotated_frag, mix, channels[3]);
        // rotated_frag.copyTo(mix);
    }

    // Scale for preview
    cv::Mat preview;
    cv::resize(M, preview, cv::Size(), 0.5, 0.5);

    cv::imshow("Display Image", preview);
    // cv::imwrite("out.png", M);
    cv::waitKey(0);
    return 0;
}