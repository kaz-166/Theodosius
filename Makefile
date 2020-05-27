Theodosious: ./src/main.o ./src/module/gpio.o
	gcc -o ./bin/Theodosius ./src/main.o ./src/module/gpio.o
main.o:	./src/main.c
	gcc -c ./src/main.c
gpio.o: ./src/module/gpio.c
	gcc -c ./src/module/gpio.c
