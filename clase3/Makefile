CC = gcc
CFLAGS = -Wall -Wextra -g -pedantic
CPPFLAGS = -Wall -Wextra -g -pedantic -lstdc++ -lm -std=c++11 -O3

OBJECTS = 

all: exec

exec: main.cpp $(OBJECTS)
	$(CC) $(CPPFLAGS) $(OBJECTS) main.cpp -o exec

clean:
	rm -r *.o *.save *~ Susanoo temp temp_vects Centroide Clusters

clean-o:
	rm -r *.o *~

clean-temp:
	rm -r temp temp_vects 

run: exec
	./ezec
