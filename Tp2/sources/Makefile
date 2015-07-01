#Project Builder
#General
CC = g++ -std=c++11
CPPFLAGS = -Wall -Wextra -g -pedantic 
CPPLDFLAGS = -lm -lstdc++
DEBUG = OFF

#ifneq($(DEBUG),OFF)
	CPPFLAGS += -D __DEBUG__
#endif

SERVER = main_server
CLIENT = main_client

SERVER_NAME = ConcusQlServer
CLIENT_NAME = ConcusQlClient

CONCURRENT_OBJECTS = queue.h message.o logger.o event_handler.h lock_file.o database_record.o

CLIENT_OBJECTS = $(CONCURRENT_OBJECTS) client.o
SERVER_OBJECTS = $(CONCURRENT_OBJECTS) database.o signal_handler.o server.o

all: $(SERVER) $(CLIENT)

%.o: %.c
	$(CC) $(CPPFLAGS) -c $<

$(SERVER): $(SERVER).cpp $(SERVER_OBJECTS)
	$(CC) $(CPPFLAGS) $(CPPLDFLAGS) $(SERVER_OBJECTS) $(SERVER).cpp -o $(SERVER_NAME)

$(CLIENT): $(CLIENT).cpp $(CLIENT_OBJECTS)
	$(CC) $(CPPFLAGS) $(CPPLDFLAGS) $(CLIENT_OBJECTS) $(CLIENT).cpp -o $(CLIENT_NAME)

clean:
	rm -f -r *.o *.save *.log *.db  *~ $(SERVER_NAME) $(CLIENT_NAME)

clean_objects:
	rm -f -r *.o *.save *~
