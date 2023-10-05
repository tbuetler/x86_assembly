////////////////////////////////////////////////////////////////
// Define all the constants for the program
///////////////////////////////////////////////////////////////
#define ADDR_BOOK_INT_SIZE  64
#define ADDR_BOOK_NAME_MAX_LEN  40
#define ADDR_BOOK_FIRST_NAME_MAX_LEN  40


//////////////////////////////////////////////////////////////
// Define the new structures to be used
//////////////////////////////////////////////////////////////
struct date {
    uint32_t year;
    uint8_t day;
    uint8_t month;
};

struct addr_book_item {
    char name[ADDR_BOOK_NAME_MAX_LEN];
    char first_name[ADDR_BOOK_FIRST_NAME_MAX_LEN];
    struct date birth_date;
};

struct addr_book {
    size_t max_size;
    size_t size;
    struct addr_book_item* array;  
};


////////////////////////////////////////////////////////////////////////////////
// Declare all functions (in header file)
///////////////////////////////////////
// create and empty address book (that contains nobody)
struct addr_book * addr_book_create_empty(void);

// delete and existing list
void addr_book_delete(struct addr_book* ab);

// insert a person at the end of the list
int addr_book_add_item(struct addr_book* ab, const char* name,const char* first_name, const char* date);

// Returns the number of persons in the address book
size_t addr_book_size(const struct addr_book* ab);

// get a pointer to the element number 'index'
struct addr_book_item* addr_book_get_element_at(struct addr_book* ab,size_t index);

// Remove the element with number 'index'
int addr_book_remove_element_at(struct addr_book* ad,size_t index);

// Remove all the elements with a given name 
int addr_book_remove_element_with_name(struct addr_book* ab,const char* name);

// Print out all the elements (in the form of CSV with semicolon as a separator)
// output is written in a stream that can be stdout or a file.
void addr_book_print(FILE * stream, const struct addr_book* ab);

// save the list in a CSV file
int addr_book_save(const char* filename,const struct addr_book* ab);

// read the list from a CSV file
struct addr_book* addr_book_create_from_file(const char* filename);

// get a new list with all the elements with a given name (new copy)
struct addr_book* addr_book_create_from_select_name(const struct addr_book* ab,const char* name);


////////////////////////////////////////////////////////////////////////////////

