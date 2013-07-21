CC = g++
CFLAGS = -Wall -fpermissive
PROG = main

SRCS = sample.cpp
LIBS = -lglut -lGL -lGLU -g

all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
