#include "../include/course.h"
#include <fstream>
#include <sstream>

// Function to read course data from a file
vector<Course> readCoursesFromFile(const string &filename)
{
    vector<Course> courses;  // Vector to store course objects

    ifstream inputFile(filename);  // Open input file
    string line;

    // Read each line from the file
    while (getline(inputFile, line))
    {
        stringstream ss(line);  // Create stringstream to parse line
        Course course;

        // Read course details from comma-separated line
        getline(ss, course.code, ',');
        getline(ss, course.title, ',');
        getline(ss, course.credit, ',');
        getline(ss, course.faculty, ',');
        getline(ss, course.batch, ',');
        
        string remaining;
        getline(ss, remaining);
        course.branches.push_back(remaining);  // Read remaining branches

        courses.push_back(course);  // Add course to vector
    }

    return courses;  // Return vector of courses
}