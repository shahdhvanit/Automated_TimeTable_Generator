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

        ss >> room.roomNumber >> room.seating >> room.roomType >> ws;
        getline(ss, room.remarks);

        rooms.push_back(room);
    }

    return rooms;
}