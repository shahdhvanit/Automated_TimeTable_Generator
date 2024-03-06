#include "../include/room.h"
#include <fstream>
#include <sstream>

vector<Room> readRoomsFromFile(const string &filename)
{
    vector<Room> rooms;

    ifstream inputFile(filename);
    string line;

    while (getline(inputFile, line))
    {
        stringstream ss(line);
        Room room;

        getline(ss, room.roomNumber, ',');
        ss >> room.seating;
        ss.ignore(); // Ignore the comma
        getline(ss, room.roomType, ',');
        getline(ss, room.remarks); // Read the entire line as remarks

        rooms.push_back(room);
    }

    return rooms;
}