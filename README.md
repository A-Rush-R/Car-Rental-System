# Car Rental System
This repository consists of the codebase for a Car Rental System for the OOPS Assignment in CPP as a part of the course CS253

## Instructions 

Run the following command to compile the programme 
```
g++ -c main.cpp -o main.o
g++ -c DateTime.cpp -o DateTime.o
g++ -c Car.cpp -o Car.o
g++ -c utils.cpp -o utils.o
g++ -c user.cpp -o user.o
g++ user.o DateTime.o main.o Car.o utils.o -o my_program
./my_program.exe
```
