#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <assert.h>
#include "addr_book_lib.h"




/** 
Contains the implementation of the functions for manipulating an Address Book.
Functions are presented in the header file.

 */


struct addr_book * addr_book_create_empty(void){

  return NULL;
  
}

int addr_book_add_item(struct addr_book* ab, const char* name,
                       const char* first_name, const char* date) {

    

    return 0;  
}
size_t addr_book_size(const struct addr_book* ab){
  return ab->size;
}

void addr_book_delete(struct addr_book* ab){


  
  return;
}

struct addr_book_item* addr_book_get_element_at(struct addr_book* ab,size_t index){




  
  return NULL;
}


int addr_book_remove_element_at(struct addr_book* ab,size_t index){





  
  return 0;
}
 
void addr_book_print(FILE * stream, const struct addr_book* ab){


  return;
}


int addr_book_save(const char* filename,const struct addr_book* ab){


  
        
  return 0;
}
int addr_book_remove_element_with_name(struct addr_book* ab,const char* name){





  
  return 0;

}

struct addr_book* addr_book_create_from_file(const char* filename){


    return NULL;


}


struct addr_book* addr_book_create_from_select_name(const struct addr_book* ab_source,const char* name){



  

    return NULL;
  

}
