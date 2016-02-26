all: http
	 
http: clean httpserver.c
	clang -g -o http httpserver.c hash_table.c linked_list.c -lpthread
	valgrind --leak-check=full --track-origins=yes ./http 3010 \
		"$(realpath ${HOME}/595/hw4/)"

strace: clean httpserver.c
	clang -g -o http httpserver.c hash_table.c linked_list.c -lpthread
	strace -f ./http 3008 \
		"$(realpath ${HOME}/595/hw4/)"

test-hash-table: clean hash_table_tester.c
	clang -g -o test hash_table_tester.c hash_table.c linked_list.c
	valgrind --leak-check=full ./test

pthread: clean pthread.c
	clang -g -o pthread pthread.c
	valgrind --leak-check=full ./pthread

clean:
	rm -rf *.o
	rm -rf http
