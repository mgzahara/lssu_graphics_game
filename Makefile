#OBJS specifies which files to compile as part of the project
OBJS = main.cpp tile.cpp game.cpp globals.cpp effect.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = DontBeJeweled

#CC specifies which compiler we're using
CC = g++

#compiler flags
CFLAGS =  $(shell sdl2-config --cflags)  -O2

#good stuff
#LIBS =  $(shell sdl2-config --libs) -lm

#this line threw err because of -lSDL2_image could not be found
LIBS = -lSDL2_image $(shell sdl2-config --libs) -lm -lSDL2_mixer

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#This is the target that compiles our executable
main : $(OBJS) Fixed_print.c
	gcc $(CFLAGS) -c Fixed_print.c
	$(CC) $(OBJS) Fixed_print.o  $(CFLAGS) $(LIBS) -o $(OBJ_NAME)
