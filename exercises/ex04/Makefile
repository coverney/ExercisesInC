# trout: trout.h trout.c util.c main.c
# 	gcc -g -o trout trout.c util.c main

util.o: util.h util.c
	gcc -g -Wall -c util.c

trout.o: trout.h trout.c
	gcc -g -Wall -c trout.c

main.o: main.c
	gcc -g -Wall -c main.c

trout: util.o trout.o main.o
	gcc -g -Wall util.o trout.o main.o -o trout
