#Project Builder
#General
CC = g++ -std=c++11 #Gaston puto no vuelvas a poner gcc
CPPFLAGS = -Wall -Wextra -g -pedantic 
CPPLDFLAGS = -lm -lstdc++
DEBUG = OFF

#ifneq($(DEBUG),OFF)
	CPPFLAGS += -D __DEBUG__
#endif

SERVER = server
CLIENT = client

CONCURRENT_OBJECTS = queue.h message.h
CLIENT_OBJECTS = $(CONCURRENT_OBJECTS)
SERVER_OBJECTS = $(CONCURRENT_OBJECTS) database.o database_record.o

all: $(SERVER) $(CLIENT)

%.o: %.c
	$(CC) $(CPPFLAGS) -c $<

$(SERVER): $(SERVER).cpp $(SERVER_OBJECTS)
	$(CC) $(CPPFLAGS) $(CPPLDFLAGS) $(SERVER_OBJECTS) $(SERVER).cpp -o $(SERVER)

$(CLIENT): $(CLIENT).cpp $(CLIENT_OBJECTS)
	$(CC) $(CPPFLAGS) $(CPPLDFLAGS) $(CLIENT_OBJECTS) $(CLIENT).cpp -o $(CLIENT)

clean:
	rm -f -r *.o *.save *.log *.db  *~ $(SERVER) $(CLIENT)

clean_objects:
	rm -f -r *.o *.save *~
