Theodosious: ./src/main.o
	gcc -o ./bin/Theodosious ./src/main.o
main.o:	./src/main.c
	gcc -c ./src/main.c
