all: main.c magicNumbers/magicLoader.c magicNumbers/magicMath.c magicNumbers/magicNumbers.c
	gcc -g -Wall -pedantic -o add main.c magicNumbers/magicLoader.c magicNumbers/magicMath.c magicNumbers/magicNumbers.c

clean: 
	$(RM) add
