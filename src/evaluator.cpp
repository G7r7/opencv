#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

struct fragment_solution
{
    int index;
    int width;
    int height;
    float angle;
};

void show_usage() {
    std::cout << "Usage: evaluator FILE_PATH [DELTA_W DELTA_H DELTA_ANGLE]" << std::endl;
}

fragment_solution parse_fragment_line(char* txt) {
    fragment_solution fragment_solution;
    std::string s(txt);
    std::string delimiter = " ";
    size_t pos = 0;
    std::vector<std::string> tokens;

    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        tokens.push_back(s.substr(0, pos));
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    fragment_solution = {
        std::stoi(tokens[0]),
        std::stoi(tokens[1]),
        std::stoi(tokens[2]),
        std::stof(tokens[3])
    };

    return fragment_solution;
}

int main(int argc, char **argv)
{
    float delta_width = 1, delta_height = 1, delta_angle = 1;
    if (argc < 2) {
        std::cout << "Missing candidate file path!" << std::endl;
        show_usage();
        return 1;
    }
    if (argc >= 5) {
        delta_width = std::atof(argv[2]);
        delta_height = std::atof(argv[3]);
        delta_angle = std::atof(argv[4]);
    }
    std::string candidate_path = argv[1];

    std::cout << "Params: candidate_file -> '" << candidate_path << "' Deltas -> " << delta_width << ":" << delta_height << ":" << delta_angle << std::endl;

    std::ifstream candidate(candidate_path);
    std::ifstream solutions("../ressources/fragments.txt");
    char solution_line[1000];
    char candidate_line[1000];

    float sum_of_correct_fragment_surfaces = 0;
    float sum_of_incorrect_fragment_surfaces = 0;

    while (solutions.getline(solution_line, 1000) && candidate.getline(candidate_line, 1000))
    {
        fragment_solution solution = parse_fragment_line(solution_line);
        fragment_solution candidate = parse_fragment_line(candidate_line);
        std::cout << "Solution : " << solution.index << " " << solution.width << " " << solution.height << " " << solution.angle << " " << std::endl;
        std::cout << "Candidat : " << candidate.index << " " << candidate.width << " " << candidate.height << " " << candidate.angle << " " << std::endl;
        float diff_width, diff_height, diff_angle;
        diff_width = std::abs(candidate.width - solution.width);
        diff_height = std::abs(candidate.height - solution.height);
        diff_angle = std::abs(candidate.angle - solution.angle);

        // Load current fragment image
        cv::Mat image = cv::imread(std::string("../ressources/frag_eroded/frag_eroded_") + std::to_string(solution.index) + std::string(".png"), cv::IMREAD_UNCHANGED);

        // Get alpha channel
        std::vector<cv::Mat> channels;
        cv::split(image, channels);
        cv::Mat alpha = channels[3];

        // Count number of non-transparents pixels (surface area)
        int surface = cv::countNonZero(alpha);
        std::cout << "Surface: " << surface << std::endl;

        // Store values
        if (diff_width <= delta_width && diff_height <= delta_height && delta_angle <= delta_angle) // Well placed
        {
            sum_of_correct_fragment_surfaces += surface;
        } else {
            sum_of_incorrect_fragment_surfaces += surface;
        }
    }
    // Calculate score
    float precision = (sum_of_correct_fragment_surfaces - sum_of_incorrect_fragment_surfaces)
        / (sum_of_correct_fragment_surfaces + sum_of_incorrect_fragment_surfaces);

    std::cout << "Final score: " << precision << "/1" << std::endl;

    return 0;
}