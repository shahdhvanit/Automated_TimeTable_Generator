#include <iostream>
#include <vector>
#include "../include/room.h"
#include "../include/course.h"

using namespace std;

int main()
{
    vector<Room> rooms = readRoomsFromFile("data/rooms.txt");
    vector<Course> courses = readCoursesFromFile("data/courses.txt");

    cout << "Room Information:\n";
    for (const Room &room : rooms)
    {
        cout << "Room Number: " << room.roomNumber << "\n";
        cout << "Seating Capacity: " << room.seating << "\n";
        cout << "Room Type: " << room.roomType << "\n";
        cout << "Remarks: " << room.remarks << "\n\n";
    }

    cout << "Course Information:\n";
    for (const Course &course : courses)
    {
        cout << "Code: " << course.code << "\n";
        cout << "Title: " << course.title << "\n";
        cout << "Credit: " << course.credit << "\n";
        cout << "Course Type: " << course.courseType << "\n";
        cout << "Faculty: " << course.faculty << "\n";
        cout << "Batch: " << course.batch << "\n";
        cout << "Branches: ";
        for (const string &branch : course.branches)
        {
            cout << branch << " ";
        }
        cout << "\n\n";
    }

    return 0;
}