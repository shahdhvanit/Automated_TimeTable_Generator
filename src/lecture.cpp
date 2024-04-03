#include "../include/lecture.h"
#include <fstream>
#include <sstream>

std::vector<LectureData> readLecturesFromFile(const std::string &filename)
{
    std::vector<LectureData> lectures;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string courseCode, courseName, faculty, roomNumber, batch, branch, courseType;
        getline(iss, courseCode, ',');
        getline(iss, courseName, ',');
        getline(iss, faculty, ',');
        getline(iss, roomNumber, ',');
        getline(iss, batch, ',');
        getline(iss, branch, ',');
        getline(iss, courseType, ',');
        lectures.push_back({courseCode, courseName, faculty, roomNumber, batch, branch, courseType});
    }
    return lectures;
}

std::vector<std::string> splitString(const std::string &input, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = input.find(delimiter, start)) != std::string::npos)
    {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(input.substr(start));
    return tokens;
}