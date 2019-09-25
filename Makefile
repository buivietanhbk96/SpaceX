src=$(wildcard *.c)
all:
	gcc $(src) -o ex -lpthread