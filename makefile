CC = g++
LIBS = -lSDL2 \
	   -lGL \
	   `sdl2-config --cflags --libs`
CPP_FILES = *.cpp
OUTPUT = Constructor

all:
	${CC} ${CPP_FILES} -o ${OUTPUT} ${LIBS}
