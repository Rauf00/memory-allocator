all: build

build:
	gcc -g bTree/bTree.c list/list.c my_malloc.c mem_test.c -o mem_test
	

run: build
	./main

valgrind: build
	valgrind --leak-check=full ./main

clean:
	rm -f main
