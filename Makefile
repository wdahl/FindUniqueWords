run: Homework_4_serial.out Homework_4_parallel.out

Homework_4_parallel.out: Homework_4_parallel.c timing.c
	gcc -g Homework_4_parallel.c timing.c -o Homework_4_parallel.out -lpthread

Homework_4_serial.out: Homework_4_serial.c timing.c
	gcc -g Homework_4_serial.c timing.c -o Homework_4_serial.out

clean:
	rm *.out