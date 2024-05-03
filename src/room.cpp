#include "../include/room.h"
#include <fstream>
#include <sstream>

// Function to read room data from a file
vector<Room> readRoomsFromFile(const string &filename)
{
    vector<Room> rooms;  // Vector to store room objects

    ifstream inputFile(filename);  // Open input file
    string line;

    // Read each line from the file
    while (getline(inputFile, line))
    {
        stringstream ss(line);  // Create stringstream to parse line
        Room room;

        getline(ss, room.roomNumber, ',');  // Read room number from line

        rooms.push_back(room);  // Add room to vector
    }

    return rooms;  // Return vector of rooms
}