# 5-address-book Program an Address Book in C

## Overview and Goal
The goal of this project is to write a library containing functions to manipulate an address book. The functions are declared in the file `addr_book_lib.h` . This file contains also the definitions of the structs that are to be used.

You have to implement the functions in the file `addr_book_lib.c`. In this file __you must implement all the functions__ that are presented. They are tested in the file `addr_book_test.c`.

### The data structure

#### The address book

An address book is built over 3 structures. First the address book itself 

	```
	struct addr_book {
		size_t max_size;
		size_t size;
		struct addr_book_item* array; 
	};
	
	```
The addresses within an address book are stored as an array. The size
of the address book (i.e. The nomber of entries it contains has to be
stored in the field `size`

#### The address book item 

Then we have a `struct` for each entry:

	struct addr_book_item {
		char name[ADDR_BOOK_NAME_MAX_LEN];
		char first_name[ADDR_BOOK_FIRST_NAME_MAX_LEN];
		struct date birth_date;
	};


Each entry stores the last name (family name), the first name (given
name) and the the day of birth of a contact. 

Third, the birth day of a contact is stored into a `struct` for itself

	
	struct addr_book_item {
		char name[ADDR_BOOK_NAME_MAX_LEN];
		char first_name[ADDR_BOOK_FIRST_NAME_MAX_LEN];
		struct date birth_date;
	};
	


## Expected Output

- You do not have the right to change both files `addr_book_lib.h` and `addr_book_test.c`.

- You have to implement the functions inside the file `addr_book_lib.c`.

- The tests must all return 0. If a test returns another value, it
means that the test failed.
