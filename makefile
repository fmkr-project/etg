etg.bin: main.c *.h
	./clean_res.sh
	$(CC) main.c -o etg.bin

$CC = gcc -Wall -Wextra 

clean:
	rm *.o
