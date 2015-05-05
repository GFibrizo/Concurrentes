#Project Builder
#General
CC = g++ -std=c++11 #Gaston puto no vuelvas a poner gcc
CPPFLAGS = -Wall -Wextra -g -pedantic 
CPPLDFLAGS = -lm -lstdc++
DEBUG = OFF

#ifneq($(DEBUG),OFF)
	CPPFLAGS += -D __DEBUG__
	CPPLDFLAGS += -D __DEBUG__
#endif

EXEC = concu_zappi

OBJECTS = lock_file.o pipe.o logger.o call_center.o

all: $(EXEC) 
		
logger.o: lock_file.o logger.h logger.cpp
	$(CC) $(CPPFLAGS) -c logger.cpp

lock_file.o: lock_file.h lock_file.cpp
	$(CC) $(CPPFLAGS) -c lock_file.cpp

pipe.o: pipe.h pipe.cpp
	$(CC) $(CPPFLAGS) -c pipe.cpp
				
call_center.o: call_center.h call_center.cpp
	$(CC) $(CPPFLAGS) -c call_center.cpp
				

$(EXEC): $(EXEC).cpp $(OBJECTS)
	$(CC) $(CPPLDFLAGS) $(OBJECTS) $(EXEC).cpp -o $(EXEC)

clean:
	rm -f -r *.o *.save *~ $(EXEC)
