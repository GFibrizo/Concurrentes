#Project Builder
#General
CC = gcc -std=c++11
CPPFLAGS = -Wall -Wextra -g -pedantic 
CPPLDFLAGS = -lm -lstdc++

EXEC = concu_zappi

OBJECTS = logger.o

all: $(EXEC) 
		
logger.o: logger.h logger.cpp
	$(CC) $(CPPFLAGS) -c logger.cpp
				
$(EXEC): $(EXEC).cpp $(OBJECTS)
	$(CC) $(CPPLDFLAGS) $(OBJECTS) $(EXEC).cpp -o $(EXEC)

clean:
	rm -f -r *.o *.save *~ $(EXEC)