#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
using namespace std;

struct Room
{
    string roomNumber;
};

vector<Room> readRoomsFromFile(const string &filename);

#endif