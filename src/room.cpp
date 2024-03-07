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
        ss.ignore();
        getline(ss, room.roomType, ',');
        getline(ss, room.remarks); 

        rooms.push_back(room);
    }

    return rooms;
}