////////////////////////////////////////////////////////////////
// Define all the constants for the program
///////////////////////////////////////////////////////////////
// Initial Size of the address book
#define ADDR_BOOK_INIT_SIZE  64
// Maximal length of a name 
#define ADDR_BOOK_NAME_MAX_LEN  40
// Maximal length of the first name
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
    size_t max_size;      // max available size for entries
    size_t size;          // the number of entries stored in the address book
    struct addr_book_item* array;  
};


////////////////////////////////////////////////////////////////////////////////
// Declare all functions (in header file)
///////////////////////////////////////
/** create and empty address book (that contains nobody)
   - the initial maximum size is defined by the constant 'ADDR_BOOK_INIT_SIZE'
   that is, at the creation of the address boook, this amount of size should be reserved

   - when the address boook is full, the reserved size maybe extended (normally doubled)
   by function addr_book_add_item()

*/
struct addr_book * addr_book_create_empty(void);

// delete and existing list
void addr_book_delete(struct addr_book* ab);


/** add an entry at the end of the list
 */
int addr_book_add_item(struct addr_book* ab, const char* name,const char* first_name, const char* date);

/** Returns the effective number of entries in the address book
 */
size_t addr_book_size(const struct addr_book* ab);

// get a pointer to the element number 'index'
struct addr_book_item* addr_book_get_element_at(struct addr_book* ab,size_t index);


/** Remove the element with number 'index'. The index is then 'compacted', that is, elements
    placed after the deleted element are shifted
 */
int addr_book_remove_element_at(struct addr_book* ad,size_t index);

// Remove all the elements with a given name 
int addr_book_remove_element_with_name(struct addr_book* ab,const char* name);


/** Print out all the elements (in the form of CSV with semicolon as a separator)
    to the given stream.

    The stream may be the standard output of the program or a file.

*/
void addr_book_print(FILE * stream, const struct addr_book* ab);


/** Explicitely save the address book in a CSV file (each entry -> one line).
    - the parameter 'filename' may include a path. We consider that the user
    has write access to this place.
 */
int addr_book_save(const char* filename,const struct addr_book* ab);



/** read a CSV file to populate the address book. 
    - each line has the same structure as shown in file that is:
    <family name>;<first name>;<year>;<month>;<day>, and should be parsed with a
    regular expression like: "%[^;];%[^;];%d;%hhd;%hhd\n"
 */
struct addr_book* addr_book_create_from_file(const char* filename);

// get a new list with all the elements with a given name (new copy)
struct addr_book* addr_book_create_from_select_name(const struct addr_book* ab,const char* name);


////////////////////////////////////////////////////////////////////////////////

