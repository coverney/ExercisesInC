# trout: trout.h trout.c util.c main.c
# 	gcc -g -o trout trout.c util.c main

util.o: util.h util.c
	gcc -g -Wall -c util.c

trout.o: trout.h util.h trout.c
	gcc -g -Wall -c trout.c

main.o: main.c trout.h util.h
	gcc -g -Wall -c main.c

trout: util.o trout.o main.o
	gcc -g -Wall util.o trout.o main.o -o trout

test_util: test_util.c util.h
	gcc test_util.c util.c -o test_util
