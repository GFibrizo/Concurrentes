#Project Builder
#General
CC = gcc -std=c++11
CPPFLAGS = -Wall -Wextra -g -pedantic 
CPPLDFLAGS = -lm -lstdc++

EXEC = exec

OBJECTS = logger.o

all: $(EXEC) 
		
logger.o: logger.h logger.cpp
	$(CC) $(CPPFLAGS) -c logger.cpp
				
$(EXEC): main.cpp $(OBJECTS)
	$(CC) $(CPPLDFLAGS) $(OBJECTS) main.cpp -o $(EXEC)

clean:
	rm -r *.o *.save *~ $(EXEC)