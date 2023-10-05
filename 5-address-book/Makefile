EXEC 	= addr_book_test 

.PHONY: all
all: $(EXEC)
CFLAGS = -Wall -Werror -g -O0

.PHONY: clean
clean:
	rm -f *.o *~
	rm -f addr_book_test
	rm -f address_book.csv
addr_book_lib.o: addr_book_lib.c
	gcc $(CFLAGS) -c addr_book_lib.c 

addr_book_test.o: addr_book_test.c
	gcc $(CFLAGS) -c addr_book_test.c 

addr_book_test: addr_book_test.o addr_book_lib.o
	gcc $(CFLAGS) addr_book_test.o addr_book_lib.o -o addr_book_test
