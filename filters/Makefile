# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: display

display: pixel_operations.o display.o

grayscale.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	
# END
