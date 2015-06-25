#Project Builder
#General
CC = g++ -std=c++11 #Gaston puto no vuelvas a poner gcc
CPPFLAGS = -Wall -Wextra -g -pedantic 
CPPLDFLAGS = -lm -lstdc++
DEBUG = OFF

#ifneq($(DEBUG),OFF)
	CPPFLAGS += -D __DEBUG__
#endif

SERVER =
CLIENT = client

CONCURRENT_OBJECTS = queue.h
OBJECTS = $(CONCURRENT_OBJECTS)

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER).cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CPPLDFLAGS) $(OBJECTS) $(SERVER).cpp -o $(SERVER)

$(CLIENT): $(CLIENT).cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CPPLDFLAGS) $(OBJECTS) $(CLIENT).cpp -o $(CLIENT)

clean:
	rm -f -r *.o *.save *.log  *~ $(SERVER) $(CLIENT)

clean_objects:
	rm -f -r *.o *.save *~
