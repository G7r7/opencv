#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>

struct fragment_solution
{
    int index;
    int width;
    int heigth;
    float angle;
};

int main(int argc, char **argv)
{
    // Empty canvas 1707*775
    int canvas_width = 1707;
    int canvas_height = 775;

    cv::Mat M(canvas_height, canvas_width, CV_8UC4, cv::Scalar(255, 255, 255, 255));

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

        // Truncate image
        // Diffs are rounded up if not whole
        int diff_top = fragment_solutions.heigth - ceil(rotated_frag.rows / 2.0) < 0 ? -1 * (fragment_solutions.heigth - ceil(rotated_frag.rows / 2.0)) : 0;
        int diff_left = fragment_solutions.width - ceil(rotated_frag.cols / 2.0) < 0 ? -1 * (fragment_solutions.width - ceil(rotated_frag.cols / 2.0)) : 0;
        int diff_bottom = fragment_solutions.heigth + ceil(rotated_frag.rows / 2.0) - canvas_height > 0 ? fragment_solutions.heigth + ceil(rotated_frag.rows / 2.0) - canvas_height : 0;
        int diff_right = fragment_solutions.width + ceil(rotated_frag.cols / 2.0) - canvas_width > 0 ? fragment_solutions.width + ceil(rotated_frag.cols / 2.0) - canvas_width : 0;

        // We crop the frag by removing the previous founded diffs
        int trunc_width = rotated_frag.cols - diff_left - diff_right;
        int trunc_height = rotated_frag.rows - diff_top - diff_bottom;

        cv::Mat truncMat = rotated_frag(cv::Rect(diff_left, diff_top, trunc_width, trunc_height));

        // Mix
        int x_original_center = fragment_solutions.width;
        int x_truncated_center = x_original_center - diff_left - diff_right;
        int x_position = x_truncated_center - truncMat.cols / 2;

        int y_original_center = fragment_solutions.heigth;
        int y_truncated_center = y_original_center - diff_top - diff_bottom;
        int y_position = y_truncated_center - truncMat.rows / 2;

        x_position = x_position < 0 ? 0 : x_position;
        y_position = y_position < 0 ? 0 : y_position;

        cv::Mat mix = M(cv::Rect(x_position, y_position, truncMat.cols, truncMat.rows));

        // Mask
        std::vector<cv::Mat> channels(4);
        cv::split(truncMat, channels);

        cv::copyTo(truncMat, mix, channels[3]);
        // truncMat.copyTo(mix);
    }

    // Scale for preview
    cv::Mat preview;
    cv::resize(M, preview, cv::Size(), 0.5, 0.5);

    cv::imshow("Display Image", preview);
    // cv::imwrite("out.png", M);
    cv::waitKey(0);
    return 0;
}