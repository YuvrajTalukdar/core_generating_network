#declare the variabler
CC=g++
LIBS= -lpthread
# use -Wall for displaying all warnings
CXXFLAGS= -std=c++17 -march=native -mtune=native -O2 -g
CXXFLAGS2= -std=c++17 -march=native -mtune=native -g
all: a.out

a.out: main.o core_class.o menu.o input_raw_to_prepared.o
	$(CC) -o a.out main.o menu.o core_class.o input_raw_to_prepared.o $(LIBS) $(OPENCVLIBS) $(CXXFLAGS)

main.o: main.cpp
	$(CC) $(CXXFLAGS) $(LIBS) -c main.cpp

menu.o: menu.cpp
	$(CC) $(CXXFLAGS) $(LIBS) -c menu.cpp

input_raw_to_prepared.o: input_raw_to_prepared.cpp
	$(CC) $(CXXFLAGS) $(LIBS) -c input_raw_to_prepared.cpp

core_class.o: core_class.cpp
	$(CC) $(CXXFLAGS) $(LIBS) -c core_class.cpp


debug: a.debug

a.debug: main_debug.o core_class_debug.o menu_debug.o input_raw_to_prepared_debug.o
	$(CC) -o a.debug main_debug.o menu_debug.o input_raw_to_prepared_debug.o core_class_debug.o $(LIBS) $(OPENCVLIBS) $(CXXFLAGS2)

main_debug.o: main.cpp
	$(CC) $(CXXFLAGS2) $(LIBS) -c main.cpp -o main_debug.o

menu_debug.o: menu.cpp
	$(CC) $(CXXFLAGS2) $(LIBS) -c menu.cpp -o menu_debug.o

input_raw_to_prepared_debug.o: input_raw_to_prepared.cpp 
	$(CC) $(CXXFLAGS2) $(LIBS) -c input_raw_to_prepared.cpp -o input_raw_to_prepared_debug.o

core_class_debug.o: core_class.cpp
	$(CC) $(CXXFLAGS2) $(LIBS) -c core_class.cpp -o core_class_debug.o


clean:
	rm -rf *o a.out a.debug
