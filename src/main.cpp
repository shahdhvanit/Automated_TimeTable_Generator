#include <iostream>
#include <vector>
#include <algorithm>
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

bool isValidAssignment(const Lecture &lecture, const vector<Lecture> &timetable, map<Timeslot, set<string>> &timeslotOccupancy)
{
    for (const auto &existingLecture : timetable)
    {
        if (isOverlap(lecture.timeslot, existingLecture.timeslot) &&
            (lecture.roomNumber == existingLecture.roomNumber ||
             lecture.faculty == existingLecture.faculty ||
             lecture.courseCode == existingLecture.courseCode ||
             (lecture.batch == existingLecture.batch && lecture.branch == existingLecture.branch)))
        {
            return false;
        }
    }

    if (timeslotOccupancy[lecture.timeslot].find(lecture.branch) != timeslotOccupancy[lecture.timeslot].end())
    {
        return false;
    }

    for (const auto &existingLecture : timetable)
    {
        if (isOverlap(lecture.timeslot, existingLecture.timeslot) &&
            lecture.batch == existingLecture.batch &&
            lecture.branch == existingLecture.branch)
        {
            return false;
        }
    }

    return true;
}

vector<Lecture> generateTimetable(const vector<Course> &courses, const vector<Room> &rooms)
{
    vector<Lecture> timetable;
    map<Timeslot, set<string>> timeslotOccupancy;

    vector<Course> shuffledCourses = courses;
    random_shuffle(shuffledCourses.begin(), shuffledCourses.end());

    vector<Timeslot> allTimeslots;
    for (int day = 1; day <= 5; ++day)
    {
        for (int slot = 1; slot <= 3; ++slot)
        {
            allTimeslots.push_back({day, slot});
        }
    }
    random_shuffle(allTimeslots.begin(), allTimeslots.end());

    map<string, int> courseAssignments;

    for (const auto &course : shuffledCourses)
    {
        for (const auto &branch : course.branches)
        {
            int assignmentsCount = courseAssignments[course.code + branch];
            int slotsAssignedInDay = 0;

            for (const auto &timeslot : allTimeslots)
            {
                bool courseAssignedOnDay = false;

                for (const auto &assignedLecture : timetable)
                {
                    if (assignedLecture.timeslot.day == timeslot.day &&
                        assignedLecture.batch == course.batch &&
                        assignedLecture.branch == branch &&
                        assignedLecture.courseCode == course.code)
                    {
                        courseAssignedOnDay = true;
                        break;
                    }
                }

                if (!courseAssignedOnDay)
                {
                    for (const auto &room : rooms)
                    {
                        Lecture lecture = {course.code, course.faculty, room.roomNumber, timeslot, course.batch, branch};

                        if (isValidAssignment(lecture, timetable, timeslotOccupancy) &&
                            find(course.branches.begin(), course.branches.end(), branch) != course.branches.end())
                        {
                            timetable.push_back(lecture);
                            timeslotOccupancy[lecture.timeslot].insert(lecture.branch);

                            ++courseAssignments[course.code + branch];
                            ++slotsAssignedInDay;

                            if (slotsAssignedInDay >= 3)
                                break;
                        }
                    }

                    if (slotsAssignedInDay >= 3)
                        break;
                }
            }
        }
    }

    return timetable;
}

void printTimetable(const vector<Lecture> &timetable)
{
    cout << setw(15) << "Course" << setw(20) << "Faculty" << setw(15) << "Room"
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
            cout << setw(15) << lecture.courseCode << setw(20) << lecture.faculty
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