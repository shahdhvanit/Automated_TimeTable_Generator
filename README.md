# Automated Time-Table Generator:

## Problem Statement:

Given slots (list of instructors, list of courses, and the course credit details), generate a weekly lecture and lab timetable with no clashes and continuous sessions of the same instructor. It might be possible to assign two instructors to the same course as well as some instructors should only be given lectures during the initial hours. Also, it should return a timetable for an instructor and a classroom/lab.

## Current Solution:

-   The current solution is implemented in C++ and generates a timetable for all branches and batch of a particular semester, it also creates timetable for a particular faculty.
-   In the current solution we haven't added data for elective courses, for elective courses the student data who opted for the courses are required, without that it is not possible to schedule elective courses properly. So, for the current solution we have only added the data for the core courses.

## Future Scope:

-   The current solution can be extended to add the data for elective courses and generate timetable for elective courses as well.
-   The current solution can be implemented with AI and algorithm to observe the pattern of timetable generation and generate an optimized timetable.

## Algorithm Description:

1. **Assign timeslots(assignTimeslots):**

    - Description: It assign a random timeslot out of total 5 timeslot in a day to each course.
    - Time Complexity: _O(n)_
    - Space Complexity: _O(1)_

2. Generate Timetable(generateTimetable):

    - Description:
        - It generates timetable for all the courses of a particular branch and batch.
        - It iterates through all the courses.
        - It checks for the number of lectures assigned to a particular course based on the l-t-p-c credits.
        - Assign lectures for each branch and batch:
            - it iterates through all required lectures.
            - For each lecture it assigns a random timeslot to the course, then it finds suitable room for the course avoiding any type of clashes.
    - Time Complexity: _O(n*m)_ where n is the number of courses and m is the number of batches.
    - Space Complexity: _O(1)_

3. GenerateCSVFiles
    - Description:
        - It first groups branches and batches
        - It then generates individual files for each branches.
    - Time Complexity: _O(n)_ where n is the number of branches.
    - Space Complexity: _O(1)_

4. GenerateCSVFilesForFaculty
    - Description:
        - It generates timetable for a particular faculty.
        - It iterates through the timetable and generates a CSV file for the faculty.
    - Time Complexity: _O(n)_ where n is the number of courses. If we generate timetable for all the individual faculties then time complexity will be _O(n*m)_ where m is the number of faculties.
    - Space Complexity: _O(1)_

## Data Structure:

1. Vectors:

    - Usage: To store the data of courses, rooms, lectures, and timeslots.
    - Reason:
        - Vector provides flexibe dynamic size.
        - For vectors time complexity for random acces is _O(1)_, time complexity for appending is _O(1)_, which makes the algorithm fast to store the data of courses, rooms, lectures, and timeslots.
        - In the worst case, the time complexity for insertion/delition is _O(n)_.

2. Maps:

    - Usage: To store the occupency data of rooms and timeslots.
    - Reason:
        - It provides a key-value pair data structure. It is efficient in searching based on keys(timeslots).
        - In the worst case, the time complexity for insertion/deletion/search is _O(log(n))_.

3. Sets:

    - Usage: To store branch Groups and abstract individual branch from it.
    - Reason:
        - For finding clash between coomon branches in individual branch and Branch groups it is efficient to use sets.
        - In the worst case, the time complexity for insertion/deletion/search is _O(log(n))_.

## How to run the code:

```sh
g++ src/main.cpp src/room.cpp src/course.cpp -o timetable
```

then

```sh
./timetable
```

## Screenshots of the output:

Screenshot of Sem-1 CS
![image](https://github.com/pranshu05/OverclockedProcessors/assets/70943732/b141ba77-b864-47ab-8792-918c22c3c0f8)

Screenshot of Faculty timetable for Amishal Modi:
![image](https://github.com/pranshu05/OverclockedProcessors/assets/70943732/34314221-e04a-42cd-9182-8cc66319983a)
