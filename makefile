main : hi prog 
	@echo "Done !"
	@echo "Run ./prog to start the Application."

hi: 
	@echo "Building Application..."

prog: utils.o user.o car.o main.o dateTime.o
	@g++ user.o dateTime.o main.o car.o utils.o -o prog

utils.o: utils.cpp utils.h
	@g++ -c utils.cpp

user.o: user.cpp user.h
	@g++ -c user.cpp

car.o: car.cpp car.h 
	@g++ -c car.cpp 

dateTime.o: dateTime.cpp dateTime.h 
	@g++ -c dateTime.cpp 


main.o: main.cpp
	@g++ -c main.cpp 

.PHONY: clean 
clean:
	@rm -f utils.o 
	@rm -f car.o 
	@rm -f main.o 
	@rm -f user.o
	@rm -f dateTime.o
	@rm -f prog 