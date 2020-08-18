CC = g++
LIBS = -lSDL2 \
	   -lGL \
	   `sdl-config --cflags --libs`
CPP_FILES = *.cpp
OUTPUT = Constructor

all:
	${CC} ${CPP_FILES} -o ${OUTPUT} ${LIBS}