#include "../include/course.h"
#include <fstream>
#include <sstream>

vector<Course> readCoursesFromFile(const string &filename)
{
    vector<Course> courses;

    ifstream inputFile(filename);
    string line;

    while (getline(inputFile, line))
    {
        stringstream ss(line);
        Course course;

        getline(ss, course.code, ',');
        getline(ss, course.title, ',');
        getline(ss, course.credit, ',');
        getline(ss, course.courseType, ',');
        getline(ss, course.faculty, ',');
        getline(ss, course.batch, ',');

        string remaining;
        getline(ss, remaining);
        course.branches.push_back(remaining);

        courses.push_back(course);
    }

    return courses;
}