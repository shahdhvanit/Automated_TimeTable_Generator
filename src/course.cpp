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

        ss >> course.code >> course.title >> course.credit >> course.courseType >> course.faculty >> course.batch >> ws;

        string branch;
        while (getline(ss, branch))
        {
            course.branches.push_back(branch);
        }

        courses.push_back(course);
    }

    return courses;
}