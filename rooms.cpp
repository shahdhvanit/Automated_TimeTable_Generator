#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Room {
    int roomNumber;
    int seating;
    string roomType;
    string remarks;
};

int main() {
    vector<Room> rooms = {
        {003, 48, "CEP", "New Classroom"},
        {102, 190, "CEP", ""},
        {103, 110, "CEP", ""},
    };

    return 0;
}