Theodosious: ./src/main.o ./src/module/gpio.o ./src/module/mem.o
	gcc -o ./bin/Theodosius ./src/main.o ./src/module/gpio.o ./src/module/mem.o -lwiringPi
main.o:	./src/main.c
	gcc -c ./src/main.c
gpio.o: ./src/module/gpio.c
	gcc -c ./src/module/gpio.c
mem.o: ./src/module/me.c
	gcc -c ./src/module/mem.c	
