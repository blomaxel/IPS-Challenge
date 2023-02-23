CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3 -lpthread

OBJECTS = main.o reading.o median.o recursive.o

distanceTest: $(OBJECTS)
	$(CC) $(OBJECTS) -lpthread -o distanceTest

reading.o: reading.c distanceTest.h
	$(CC) $(CFLAGS) -c reading.c
median.o: median.c distanceTest.h
	$(CC) $(CFLAGS) -c median.c

recursive.o: recursive.c distanceTest.h
	$(CC) $(CFLAGS) -c  recursive.c

main.o: main.c distanceTest.h reading.c
	$(CC) $(CFLAGS) -c main.c

.PHONY: clean

clean:
	-rm -f $(OBJECTS) distanceTest

