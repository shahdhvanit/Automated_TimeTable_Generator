#include <iostream>
#include <vector>
#include "room.h"

using namespace std;

int main()
{
    // Read room data from file
    vector<Room> rooms = readRoomsFromFile("rooms.txt");

    // Display room data (for testing purposes)
    for (const Room& room : rooms)
    {
        cout << "Room Number: " << room.roomNumber << "\n";
        cout << "Seating Capacity: " << room.seating << "\n";
        cout << "Room Type: " << room.roomType << "\n";
        cout << "Remarks: " << room.remarks << "\n\n";
    }

    // Rest of your main code

    return 0;
}
