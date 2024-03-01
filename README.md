# Car Rental System
This repository consists of the codebase for a Car Rental System for the OOPS Assignment in CPP as a part of the course CS253

## Contents
This repository has the following contents &rarr;
1. [`constants.h`](/constants.h) file which consists of all the constants used in the programme -- >
    - Penalty fraction for the users for late return of cars
    - Beginning ID for the various types of users
    - Encoding of the conditions for the car (numerically closer conditions are more closer)
    - Repair reward, given to the employee for repairing a car (one unit per change in condition)
    - Discount for the employees 
    - Defualt values for record (when average is not available)

## Instructions 

Run the following command to compile and run the programme 
```bash
g++ -c main.cpp -o main.o
g++ -c DateTime.cpp -o DateTime.o
g++ -c Car.cpp -o Car.o
g++ -c utils.cpp -o utils.o
g++ -c user.cpp -o user.o
g++ user.o DateTime.o main.o Car.o utils.o -o my_program
./my_program.exe
```

## Details

- Repairing a car rewards the employee by increasing their record
- An unexpected input will cause the program to terminate, but databases will be saved

## Assumptions 

- It is assumed that there are only 4 types of conditions for a car (in order of increasing severity of damage) &rarr;
    - Fine 
    - Minor Scratches
    - Light Damage
    - Heavy Damage
- The default cap on number of users (per type) is 100000, it can be changed in the [`constants.h`](/constants.h) file (it would require changing the id of all the objects in the database if done so)

## Pending 

- Update functions for customer,manager and employee