#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <ctime>
#include <iomanip>
#include "../include/course.h"
#include "../include/room.h"

using namespace std;

struct Timeslot
{
    int day;
    int slot;

    friend bool operator<(const Timeslot &lhs, const Timeslot &rhs)
    {
        if (lhs.day == rhs.day)
        {
            return lhs.slot < rhs.slot;
        }
        return lhs.day < rhs.day;
    }
};

struct Lecture
{
    string courseCode;
    string faculty;
    string roomNumber;
    Timeslot timeslot;
    string batch;
    string branch;
};

bool isOverlap(const Timeslot &t1, const Timeslot &t2)
{
    return (t1.day == t2.day && t1.slot == t2.slot);
}

bool isValidAssignment(const Lecture &lecture, const vector<Lecture> &timetable, map<Timeslot, set<string>> &timeslotOccupancy, int &maxLecturesPerCourseAndBranch)
{
    auto splitBranches = [](const string &branches)
    {
        set<string> result;
        stringstream ss(branches);
        string branch;
        while (getline(ss, branch, ','))
        {
            result.insert(branch);
        }
        return result;
    };

    for (const auto &existingLecture : timetable)
    {
        set<string> commonBranches;
        set<string> lectureBranchSet = splitBranches(lecture.branch);
        set<string> existingLectureBranchSet = splitBranches(existingLecture.branch);

        set_intersection(
            lectureBranchSet.begin(), lectureBranchSet.end(),
            existingLectureBranchSet.begin(), existingLectureBranchSet.end(),
            inserter(commonBranches, commonBranches.begin()));

        if (isOverlap(lecture.timeslot, existingLecture.timeslot) &&
            (lecture.roomNumber == existingLecture.roomNumber ||
             lecture.faculty == existingLecture.faculty ||
             lecture.courseCode == existingLecture.courseCode ||
             (lecture.batch == existingLecture.batch && !commonBranches.empty())))
        {
            return false;
        }
    }

    if (timeslotOccupancy[lecture.timeslot].find(lecture.branch) != timeslotOccupancy[lecture.timeslot].end())
    {
        return false;
    }

    if (maxLecturesPerCourseAndBranch <= 0)
    {
        return false;
    }

    return true;
}

vector<Lecture> generateTimetable(const vector<Course> &courses, const vector<Room> &rooms)
{
    vector<Lecture> timetable;
    map<Timeslot, set<string>> timeslotOccupancy;
    set<string> assignedCourses;

    vector<Course> shuffledCourses = courses;
    random_shuffle(shuffledCourses.begin(), shuffledCourses.end());

    map<string, int> courseAssignments;
    map<string, int> maxLecturesPerCourse;

    for (const auto &course : courses)
    {
        for (const auto &branch : course.branches)
        {
            maxLecturesPerCourse[course.code + branch] = 3;
        }
    }

    for (const auto &course : shuffledCourses)
    {
        for (const auto &branch : course.branches)
        {
            int assignmentsCount = courseAssignments[course.code + branch];
            int maxLecturesPerCourseAndBranch = maxLecturesPerCourse[course.code + branch];

            for (const auto &room : rooms)
            {
                for (int day = 1; day <= 5; ++day)
                {
                    for (int slot = 1; slot <= 5; ++slot)
                    {
                        const Timeslot timeslot = {day, slot};
                        const string &roomNumber = room.roomNumber;

                        Lecture lecture = {course.code, course.faculty, roomNumber, timeslot, course.batch, branch};

                        if (isValidAssignment(lecture, timetable, timeslotOccupancy, maxLecturesPerCourseAndBranch))
                        {
                            timetable.push_back(lecture);
                            timeslotOccupancy[lecture.timeslot].insert(lecture.branch);

                            assignedCourses.insert(course.code);

                            ++courseAssignments[course.code + branch];
                            --maxLecturesPerCourseAndBranch;

                            break;
                        }
                    }
                }
            }
        }
    }

    for (const auto &course : courses)
    {
        for (const auto &branch : course.branches)
        {
            if (assignedCourses.find(course.code + branch) == assignedCourses.end())
            {
                Lecture defaultLecture = {course.code, course.faculty, "", {1, 1}, course.batch, branch};
                timetable.push_back(defaultLecture);
            }
        }
    }

    return timetable;
}

void printTimetable(const vector<Lecture> &timetable)
{
    cout << setw(10) << "Course" << setw(20) << "Faculty" << setw(15) << "Room"
         << setw(10) << "Day" << setw(10) << "Slot" << setw(15) << "Batch" << setw(15) << "Branch" << endl;

    map<pair<string, string>, vector<Lecture>> groupedLectures;
    for (const auto &lecture : timetable)
    {
        groupedLectures[{lecture.batch, lecture.branch}].push_back(lecture);
    }

    for (const auto &entry : groupedLectures)
    {
        const auto &lectures = entry.second;
        for (const auto &lecture : lectures)
        {
            cout << setw(10) << lecture.courseCode << setw(20) << lecture.faculty
                 << setw(15) << lecture.roomNumber << setw(10) << lecture.timeslot.day
                 << setw(10) << lecture.timeslot.slot << setw(15) << lecture.batch
                 << setw(15) << lecture.branch << endl;
        }
        cout << string(100, '-') << endl;
    }
}

int main()
{
    vector<Course> courses = readCoursesFromFile("data/courses.txt");
    vector<Room> rooms = readRoomsFromFile("data/rooms.txt");

    srand(static_cast<unsigned int>(time(0)));

    vector<Lecture> timetable = generateTimetable(courses, rooms);

    sort(timetable.begin(), timetable.end(), [](const Lecture &lhs, const Lecture &rhs)
         {
             if (lhs.timeslot.day == rhs.timeslot.day)
             {
                 return lhs.timeslot.slot < rhs.timeslot.slot;
             }
             return lhs.timeslot.day < rhs.timeslot.day; });

    printTimetable(timetable);

    return 0;
}