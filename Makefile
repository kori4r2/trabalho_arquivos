# Alisson Mateus de Oliveira Magalhães - nUSP 8066287
# Ítalo Tobler Silva - nUSP 8551910

CFLAGS = -g -Wall

all: build

build:
	gcc -o program *.c -I./ $(CFLAGS)

clean:
	rm -f *.o
	rm -f *.dif
	rm -f *.zip
	rm -f test*.out
	rm -f program
	rm -f *.data
	clear

run: build
	./program

.zip:
	zip trabalho_arquivos.zip *.c *.h Makefile

debug: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./program #< 1.in > test1.out
	#cat test1.out

