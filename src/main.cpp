#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <set>
#include <ctime>
#include <iomanip>
#include <fstream>
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
            lecture.batch == existingLecture.batch &&
            !commonBranches.empty())
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
    map<int, set<string>> lecturesAssignedPerDay;

    vector<Course> shuffledCourses = courses;
    random_shuffle(shuffledCourses.begin(), shuffledCourses.end());

    vector<pair<Timeslot, string>> allSlotsWithRooms;

    for (int day = 1; day <= 5; ++day)
    {
        for (int slot = 1; slot <= 5; ++slot)
        {
            for (const auto &room : rooms)
            {
                allSlotsWithRooms.push_back({{day, slot}, room.roomNumber});
            }
        }
    }

    random_shuffle(allSlotsWithRooms.begin(), allSlotsWithRooms.end());

    map<string, int> courseAssignments;

    for (const auto &course : shuffledCourses)
    {
        int lectures = stoi(course.credit.substr(0, course.credit.find("-")));
        int lectureCount = 0;

        for (const auto &branch : course.branches)
        {
            for (int i = 0; i < lectures; ++i)
            {
                for (const auto &slotWithRoom : allSlotsWithRooms)
                {
                    const auto &timeslot = slotWithRoom.first;
                    const auto &roomNumber = slotWithRoom.second;

                    Lecture lecture = {course.code, course.faculty, roomNumber, timeslot, course.batch, branch};

                    if (isValidAssignment(lecture, timetable, timeslotOccupancy) &&
                        find(course.branches.begin(), course.branches.end(), branch) != course.branches.end() &&
                        lecturesAssignedPerDay[timeslot.day].count(lecture.courseCode) == 0)
                    {
                        timetable.push_back(lecture);
                        timeslotOccupancy[lecture.timeslot].insert(lecture.branch);
                        lecturesAssignedPerDay[timeslot.day].insert(lecture.courseCode);

                        ++courseAssignments[course.code + branch];
                        ++lectureCount;

                        break;
                    }
                }
            }
        }

        for (int day = 1; day <= 5; ++day)
        {
            lecturesAssignedPerDay[day].clear();
        }
    }

    return timetable;
}

void generateCSVFiles(const vector<Lecture> &timetable)
{
    map<pair<string, string>, vector<Lecture>> batchBranchLectures;
    for (const auto &lecture : timetable)
    {
        vector<string> branches;
        stringstream ss(lecture.branch);
        string branch;
        while (getline(ss, branch, ','))
        {
            branches.push_back(branch);
        }

        for (const auto &branch : branches)
        {
            batchBranchLectures[{lecture.batch, branch}].push_back(lecture);
        }
    }

    for (const auto &entry : batchBranchLectures)
    {
        const auto &batchBranch = entry.first;
        const auto &lectures = entry.second;

        string filename = "timetable_" + batchBranch.first + "_" + batchBranch.second + ".csv";
        ofstream file(filename);

        file << "Slots,Monday,Tuesday,Wednesday,Thursday,Friday\n";

        for (int slot = 1; slot <= 5; ++slot)
        {
            file << slot;

            for (int day = 1; day <= 5; ++day)
            {
                file << ",";

                auto it = find_if(lectures.begin(), lectures.end(), [=](const Lecture &l)
                                  { return l.timeslot.day == day && l.timeslot.slot == slot; });

                if (it != lectures.end())
                {
                    const Lecture &lecture = *it;
                    file << lecture.courseCode << "("
                         << lecture.faculty << "|"
                         << lecture.roomNumber << ")";
                }
                else
                {
                    file << " ";
                }
            }

            file << "\n";
        }
        file.close();
    }
}

int main()
{
    cout << "Enter semester type: (autumn/winter)" << endl;
    string semester;
    cin >> semester;

    if (semester != "autumn" && semester != "winter")
    {
        cout << "Invalid semester type" << endl;
        return 1;
    }

    string inputFile = "data/" + semester + ".txt";

    ifstream input(inputFile);

    if (!input)
    {
        cerr << "Error: Unable to open file " << inputFile << endl;
        return 1;
    }

    vector<Course> courses = readCoursesFromFile(inputFile);
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

    generateCSVFiles(timetable);

    return 0;
}