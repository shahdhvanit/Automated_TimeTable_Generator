#ifndef LECTURE_H
#define LECTURE_H

#include <string>
#include <vector>

struct LectureData
{
    std::string courseCode;
    std::string courseName;
    std::string faculty;
    std::string roomNumber;
    std::string batch;
    std::string branch;
    std::string courseType;
};

std::vector<LectureData> readLecturesFromFile(const std::string &filename);
std::vector<std::string> splitString(const std::string &input, const std::string &delimiter);

#endif