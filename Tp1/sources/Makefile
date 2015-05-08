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

CONCURRENT_OBJECTS = lock_file.o pipe.o semaphore.o fifo.o reader_fifo.o writer_fifo.o
OBJECTS = $(CONCURRENT_OBJECTS) logger.o call_center.o kitchen.o

all: $(EXEC) 

logger.o: lock_file.o logger.h logger.cpp
	$(CC) $(CPPFLAGS) -c logger.cpp

kitchen.o: kitchen.h kitchen.cpp
	$(CC) $(CPPFLAGS) -c kitchen.cpp

lock_file.o: lock_file.h lock_file.cpp
	$(CC) $(CPPFLAGS) -c lock_file.cpp

pipe.o: pipe.h pipe.cpp
	$(CC) $(CPPFLAGS) -c pipe.cpp
				
call_center.o: call_center.h call_center.cpp
	$(CC) $(CPPFLAGS) -c call_center.cpp
				
semaphore.o: semaphore.h semaphore.cpp
	$(CC) $(CPPFLAGS) -c semaphore.cpp
	
fifo.o: fifo.h fifo.cpp
	$(CC) $(CPPFLAGS) -c fifo.cpp

reader_fifo.o: reader_fifo.h reader_fifo.cpp
	$(CC) $(CPPFLAGS) -c reader_fifo.cpp
	
writer_fifo.o: writer_fifo.h writer_fifo.cpp
	$(CC) $(CPPFLAGS) -c writer_fifo.cpp
	
$(EXEC): $(EXEC).cpp $(OBJECTS)
	$(CC) $(CPPLDFLAGS) $(OBJECTS) $(EXEC).cpp -o $(EXEC)

clean:
	rm -f -r *.o *.save *.log  *~ $(EXEC)

clean_objects:
	rm -f -r *.o *.save *~
