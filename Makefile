Theodosious: ./src/main.o
	gcc -o ./bin/Theodosius ./src/main.o
main.o:	./src/main.c
	gcc -c ./src/main.c
