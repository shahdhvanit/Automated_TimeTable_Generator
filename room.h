#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
using namespace std;

struct Room
{
    int roomNumber;
    int seating;
    string roomType;
    string remarks;
};

vector<Room> readRoomsFromFile(const string& filename);

#endif
