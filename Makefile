CC = gcc
CFLAGS = -Wall -Wextra -pthread  

all: findtopk findtopk_mqueue findtopk_thread

findtopk: findtopk.c
	$(CC) $(CFLAGS) -o findtopk findtopk.c

findtopk_mqueue: findtopk_mqueue.c
	$(CC) $(CFLAGS) -o findtopk_mqueue findtopk_mqueue.c -lrt 

findtopk_thread: findtopk_thread.c
	$(CC) $(CFLAGS) -o findtopk_thread findtopk_thread.c -pthread 

clean:
	rm -f findtopk findtopk_mqueue findtopk_thread

.PHONY: all clean
