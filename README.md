# Car Rental System
This repository consists of the codebase for a Car Rental System for the OOPS Assignment in CPP as a part of the course CS253

## Contents
This repository has the following contents &rarr;
1. [`constants.h`](/constants.h) file which consists of all the constants used in the programme &rarr;
    - Factor of the rent need to be paid as fine per day for returning the car late
    - Beginning ID for the various types of users
    - Cap on the number of users
    - Encoding of the conditions for the car (numerically closer conditions are more closer)
    - Repair reward, given to the employee for repairing a car (one unit per change in condition)
    - Discount for the employees 
    - Defualt values for record (when average is not available)
    - Penalty for damaging the car
2. [`car.h`](/car.h) and [`car.cpp`](/car.cpp) files containing the car class with attributes and operations
3. [`user.h`](/user.h) and [`user.cpp`](/user.cpp) files containing the user superclass, customer, employee and manager class with attributes and operations
4. [`dateTime.h`](/dateTime.h) and [`dateTime.cpp`](/dateTime.cpp) files containing the dateTime class with attributes and operations
5. [`utils.h`](/utils.h) and [`utils.cpp`](/utils.cpp) files containg utiltiy functions &rarr;
    - `parse_date` function to read date and time inputs from user
    - Overloaded `-` operator to subtract dates
6. [`main.cpp`](/main.cpp) file containing the driver function for the program
7. `.txt` files for the databases &rarr;
    - [`cars.txt`](/cars.txt)
    - [`customers.txt`](/customers.txt)
    - [`employees.txt`](/employees.txt)
    - [`managers.txt`](/managers.txt)

## Instructions 
- Run the following make command to compile and run the programme 
```bash
make 
```
- To clean the files, run the following command 
```bash
make clean 
```

## Details
- Employees get a discount of 15% for renting cars
- Employees have the option of repairing a car, which rewards them by increasing their record
- Only customers can register on their own, registering an employee would require manager to do it.
- Functionality of adding/deleting/updating is implemented but not accessible to any user/manager and must be done by manually adding in the database
- An unexpected input will cause the program to terminate, but databases will be saved

## Assumptions 
- It is assumed that there are only 4 types of conditions for a car (in order of increasing severity of damage) &rarr;
    - Fine 
    - Minor Scratches
    - Light Damage
    - Heavy Damage
- The default cap on number of users (per type) is 100000, it can be changed in the [`constants.h`](/constants.h) file (it would require changing the id of all the objects in the database if done so).
- At the time of return it is assumed that the condition of the car returned will be same or worse than the condition at the time of renting.
- Whenever a string input is given it is assumped to have no spaces.
- It is assumed that the user will give a date in present/future.
