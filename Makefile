all: build

build:
	gcc -g bTree/bTree.c list/list.c my_malloc.c mem_test.c -o mem_test
	
run: build
	./mem_test

valgrind: build
	valgrind --leak-check=full ./mem_test

clean:
	rm -f mem_test
