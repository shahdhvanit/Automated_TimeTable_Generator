#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

vector<string> courseCodeInput;
vector<string> courseNameInput;
vector<string> facultyInput;
vector<string> numberOfLecturesInput;
vector<string> roomInput;
vector<string> batchInput;
vector<string> branchGroupInput;

void PrintCourseData();

int main()
{
    string semester;
    cout << "Enter the semester (winter/autumn): ";
    cin >> semester;

    string filename = (semester == "winter") ? "data/winter.txt" : "data/autumn.txt";
    ifstream input(filename);

    if (!input)
    {
        cerr << "Error: Unable to open file " << filename << endl;
        return 1;
    }

    string perticularline;
    while (getline(input, perticularline))
    {
        istringstream iss(perticularline);
        string code, name, faculty, numberoflectures, room, batch, branchGroup;
        if (getline(iss, code, ',') && getline(iss, name, ',') && getline(iss, faculty, ',') &&
            getline(iss, numberoflectures, ',') && getline(iss, room, ',') && getline(iss, batch, ',') &&
            getline(iss, branchGroup, ','))
        {
            courseCodeInput.push_back(code);
            courseNameInput.push_back(name);
            facultyInput.push_back(faculty);
            numberOfLecturesInput.push_back(numberoflectures);
            roomInput.push_back(room);
            batchInput.push_back(batch);
            branchGroupInput.push_back(branchGroup);
        }
    }

    PrintCourseData();

    return 0;
}

void PrintCourseData() {
    cout << "Course Data:" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Course Code\tCourse Name\tFaculty\tcredits\tRoom\tSemester\tBranch Group\tBranches" << endl;
    cout << "----------------------------------------------" << endl;
    for (size_t i = 0; i < courseCodeInput.size(); ++i) {
        cout << courseCodeInput[i] << "\t\t" << courseNameInput[i] << "\t\t" << facultyInput[i] << "\t\t" 
             << numberOfLecturesInput[i] << "\t\t" << roomInput[i] << "\t\t" << batchInput[i] << "\t\t" 
             << branchGroupInput[i] << "\t\t";
        
        if (branchGroupInput[i].find(';') != string::npos) {
            stringstream ss(branchGroupInput[i]);
            string branch;
            while (getline(ss, branch, ';')) {
                cout << branch << ", ";
            }
        } else {
            cout << branchGroupInput[i];
        }
        cout << endl;
    }
    cout << "----------------------------------------------" << endl;
}