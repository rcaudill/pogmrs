CC = gcc
CFLAGS = -ansi -Wall
                                                                                                         
all: sweep_up time sound_library

sweep_up: sweep_up.o
	$(CC) $(CFLAGS) sweep_up.o -o sweep_up -lm

sweep_up.o: sweep_up.c
	$(CC) $(CFLAGS) -c sweep_up.c

time: time.o
	$(CC) $(CFLAGS) time.o -o time -lm

time.o: time.c
	$(CC) $(CFLAGS) -c time.c

sound_library: sound_library.o
	$(CC) $(CFLAGS) sound_library.o -o sound_library -lm

sound_library.o: sound_library.c
	$(CC) $(CFLAGS) -c sound_library.c

clean:
	rm *.o *~ *#
