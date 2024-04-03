#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <ctime>
#include <iomanip>
#include "../include/lecture.h"

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
    vector<string> batches;
    vector<string> branches;
    string courseType;
};

bool isOverlap(const Timeslot &t1, const Timeslot &t2)
{
    return (t1.day == t2.day && t1.slot == t2.slot);
}

bool isValidAssignment(const Lecture &lecture, const vector<Lecture> &timetable, map<Timeslot, set<string>> &timeslotOccupancy)
{
    map<string, int> lectureDaysCount;

    for (const auto &existingLecture : timetable)
    {
        for (const auto &batch : existingLecture.batches)
        {
            for (const auto &branch : existingLecture.branches)
            {
                if (batch == lecture.batches[0] && branch == lecture.branches[0])
                {
                    lectureDaysCount[existingLecture.courseCode]++;
                }
            }
        }

        for (const auto &batch : lecture.batches)
        {
            for (const auto &branch : lecture.branches)
            {
                if (isOverlap(lecture.timeslot, existingLecture.timeslot) &&
                    (lecture.roomNumber == existingLecture.roomNumber ||
                     lecture.faculty == existingLecture.faculty ||
                     lecture.courseCode == existingLecture.courseCode ||
                     (batch == existingLecture.batches[0] && branch == existingLecture.branches[0] && lecture.courseType == "elective")))
                {
                    return false;
                }
            }
        }
    }

    if (lectureDaysCount.size() >= 5)
    {
        return false;
    }

    return true;
}

vector<Lecture> generateTimetable(const vector<Lecture> &lectures)
{
    vector<Lecture> timetable;
    map<Timeslot, set<string>> timeslotOccupancy;

    srand(static_cast<unsigned int>(time(0)));

    vector<Lecture> shuffledLectures = lectures;
    random_shuffle(shuffledLectures.begin(), shuffledLectures.end());

    for (const auto &lecture : shuffledLectures)
    {
        Lecture assignedLecture = lecture;

        for (int day = 1; day <= 5; ++day)
        {
            for (int slot = 1; slot <= 5; ++slot)
            {
                const Timeslot timeslot = {day, slot};

                assignedLecture.timeslot = timeslot;

                if (isValidAssignment(assignedLecture, timetable, timeslotOccupancy))
                {
                    timetable.push_back(assignedLecture);
                    timeslotOccupancy[assignedLecture.timeslot].insert(assignedLecture.courseCode);

                    break;
                }
            }
        }
    }

    return timetable;
}

void printTimetable(const vector<Lecture> &timetable)
{
    cout << setw(10) << "Course" << setw(20) << "Faculty" << setw(15) << "Room"
         << setw(10) << "Day" << setw(10) << "Slot" << setw(15) << "Batch" << setw(15) << "Branch" << setw(15) << "Course Type" << endl;

    map<string, map<string, vector<Lecture>>> lecturesByBatchAndBranch;

    for (const auto &lecture : timetable)
    {
        for (const auto &batch : lecture.batches)
        {
            for (const auto &branch : lecture.branches)
            {
                string key = batch + "_" + branch;
                lecturesByBatchAndBranch[key][lecture.courseCode].push_back(lecture);
            }
        }
    }

    for (const auto &batchAndBranch : lecturesByBatchAndBranch)
    {
        string batch = batchAndBranch.first.substr(0, batchAndBranch.first.find('_'));
        string branch = batchAndBranch.first.substr(batchAndBranch.first.find('_') + 1);

        cout << string(100, '-') << endl;
        cout << "Batch: " << batch << ", Branch: " << branch << endl;

        for (const auto &course : batchAndBranch.second)
        {
            cout << "Course: " << course.first << endl;

            for (const auto &lecture : course.second)
            {
                cout << setw(10) << course.first << setw(20) << lecture.faculty
                     << setw(15) << lecture.roomNumber << setw(10) << lecture.timeslot.day
                     << setw(10) << lecture.timeslot.slot << setw(15) << batch
                     << setw(15) << branch << setw(15) << lecture.courseType << endl;
            }
        }
    }
}

int main()
{
    vector<LectureData> lectures = readLecturesFromFile("data/lectures.txt");

    vector<Lecture> courses;
    for (const auto &lecture : lectures)
    {
        Lecture newLecture;
        newLecture.courseCode = lecture.courseCode;
        newLecture.faculty = lecture.faculty;
        newLecture.roomNumber = lecture.roomNumber;
        newLecture.batches = splitString(lecture.batch, ";");
        newLecture.branches = splitString(lecture.branch, ";");
        newLecture.courseType = lecture.courseType;
        courses.push_back(newLecture);
    }

    vector<Lecture> timetable = generateTimetable(courses);

    printTimetable(timetable);

    return 0;
}
