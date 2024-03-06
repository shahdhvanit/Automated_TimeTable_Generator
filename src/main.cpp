#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <ctime>
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
};

bool isOverlap(const Timeslot &t1, const Timeslot &t2)
{
    return (t1.day == t2.day && t1.slot == t2.slot);
}

bool isValidAssignment(const Lecture &lecture, const vector<Lecture> &timetable)
{
    for (const auto &existingLecture : timetable)
    {
        if (isOverlap(lecture.timeslot, existingLecture.timeslot) &&
            (lecture.roomNumber == existingLecture.roomNumber ||
             lecture.faculty == existingLecture.faculty ||
             lecture.courseCode == existingLecture.courseCode))
        {
            return false;
        }
    }
    return true;
}

vector<Lecture> generateTimetable(const vector<Course> &courses, const vector<Room> &rooms)
{
    vector<Lecture> timetable;

    vector<Course> shuffledCourses = courses;
    random_shuffle(shuffledCourses.begin(), shuffledCourses.end());

    vector<Timeslot> allTimeslots;
    for (int day = 1; day <= 2; ++day)
    {
        for (int slot = 1; slot <= 3; ++slot)
        {
            allTimeslots.push_back({day, slot});
        }
    }
    random_shuffle(allTimeslots.begin(), allTimeslots.end());

    map<string, int> courseAssignments;

    map<Timeslot, set<string>> roomAvailability;

    for (const auto &course : shuffledCourses)
    {
        int assignmentsCount = courseAssignments[course.code];

        for (const auto &timeslot : allTimeslots)
        {
            for (const auto &room : rooms)
            {
                Lecture lecture = {course.code, course.faculty, room.roomNumber, timeslot};

                if (isValidAssignment(lecture, timetable) &&
                    roomAvailability[timeslot].find(room.roomNumber) == roomAvailability[timeslot].end())
                {
                    timetable.push_back(lecture);
                    roomAvailability[timeslot].insert(room.roomNumber);

                    ++courseAssignments[course.code];

                    if (courseAssignments[course.code] >= 1)
                        break;

                    break;
                }
            }

            if (courseAssignments[course.code] >= 1)
                break;
        }
    }

    return timetable;
}

int main()
{
    vector<Course> courses = readCoursesFromFile("data/courses.txt");
    vector<Room> rooms = readRoomsFromFile("data/rooms.txt");

    srand(static_cast<unsigned int>(time(0)));

    vector<Lecture> timetable = generateTimetable(courses, rooms);

    sort(timetable.begin(), timetable.end(), [](const Lecture &lhs, const Lecture &rhs) {
        return lhs.roomNumber < rhs.roomNumber;
    });

    for (const auto &lecture : timetable)
    {
        cout << "Course: " << lecture.courseCode << ", Faculty: " << lecture.faculty
             << ", Room: " << lecture.roomNumber << ", Day: " << lecture.timeslot.day
             << ", Slot: " << lecture.timeslot.slot << endl;
    }

    return 0;
}