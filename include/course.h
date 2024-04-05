#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <vector>
using namespace std;

struct Course
{
    string code;
    string title;
    string credit;
    string faculty;
    string batch;
    vector<string> branches;
};

vector<Course> readCoursesFromFile(const string &filename);

#endif