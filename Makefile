src=$(wildcard *.c)
all:
	gcc $(src) -o SpaceX -lpthread