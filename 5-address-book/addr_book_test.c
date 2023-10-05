#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <assert.h>
#include "addr_book_lib.h"




// ==============================================================================



int test1(){
    int res=0;
    struct addr_book* ad;
    ad = addr_book_create_empty();

    res=addr_book_add_item(ad,"Muster","Hans","1.01.2000");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }

    res=addr_book_add_item(ad, "Mayer","Jan","1.01.1990");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }
    addr_book_delete(ad);
    return 0;

}

int test2(){
    int res=0;
    struct addr_book* ad;
    ad = addr_book_create_empty();

    res=addr_book_add_item(ad,"Muster","Hans","1.01.2000");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }

    res=addr_book_add_item(ad, "Mayer","Jan","1.01.1990");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }

    struct addr_book_item* elem1;
    elem1 = addr_book_get_element_at(ad,1);
    if(NULL == elem1){
      puts("error reading the element with index 1");
      return 3;
    }
    int cmp = strcmp(elem1->name,"Mayer");
    if(0!=cmp){
      puts("element index 1 has not the right name");
      return 4;
    }
    puts("----test 2 ------");
    addr_book_print(stdout,ad);
    puts("---------------");

    elem1 = addr_book_get_element_at(ad,2);
    if(NULL != elem1){
      puts("error : there is a problem with out of bound index");
      return 3;
    }
   

    addr_book_delete(ad);
    return 0;
}

int test3(){
    int res=0;
    struct addr_book* ad;
    ad = addr_book_create_empty();

    res=addr_book_add_item(ad,"Muster","Hans","1.01.2000");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }

    res=addr_book_add_item(ad, "Mayer","Jan","1.01.1990");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }

    res=addr_book_add_item(ad, "Mueller","Eric","1.01.1999");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }


    res=addr_book_add_item(ad, "Mueller","Muriel","31.12.1999");

    if(0!=res){
        puts("error entering a new person");
        return res;
    }

    struct addr_book_item* elem1;
    elem1 = addr_book_get_element_at(ad,1);
    if(NULL == elem1){
      puts("error reading the element with index 1");
      return 3;
    }

    int cmp = strcmp(elem1->name,"Mayer");
    if(0!=cmp){
      puts("element index 1 has not the right name");
      return 4;
    }
    res = addr_book_remove_element_at(ad,1);

    if(0!=res){
        puts("error removing person");
        return res;
    }

    struct addr_book_item* elem0;
    elem0 = addr_book_get_element_at(ad,0);
    if(NULL == elem1){
      puts("error reading the element with index 0 (after remove)");
      return 3;
    }
    elem1 = addr_book_get_element_at(ad,1);
    if(NULL == elem1){
      puts("error reading the element with index 1 (after remove)");
      return 3;
    }

    cmp = strcmp(elem0->name,"Muster");
    int cmp2 = strcmp(elem1->name,"Mueller");
    if(0!=cmp || 0 != cmp2){
      puts("error with names after remove");
      return 5;
      
    }
    if(3!=addr_book_size(ad)){
      puts("error with size");
      return 5;

    }
    puts("----test 3 ------");
    addr_book_print(stdout,ad);
    puts("---------------");
    res=addr_book_save("address_book.csv",ad);

    if(0!=res){
        puts("error saving address book");
        return res;
    }

    res = addr_book_remove_element_with_name(ad,"Mustermann");

    if(0==res){
        puts("error of return code if removing unexisting person");
        return res;
    }

    res = addr_book_remove_element_with_name(ad,"Muster");

    if(0!=res){
        puts("error removing person");
        return res;
    }

    puts("----test 3 (after remove ------");
    addr_book_print(stdout,ad);
    puts("---------------");

    
    addr_book_delete(ad);
    return 0;
}


int test4(void){
  struct addr_book* ab = addr_book_create_from_file("address_book_long.csv");
  if(NULL == ab){
    puts("Error: Problem creating the Address book from file.");
    return 1;
  }
  printf("Size of the address book %ld\n",addr_book_size(ab));
  addr_book_delete(ab);
  return 0;
}

int test5(void){
  struct addr_book* ab = addr_book_create_from_file("address_book_long.csv");
  if(NULL == ab){
    puts("Error: Problem creating the Address book from file.");
    return 1;
  }
  fprintf(stdout,"Size created address book = %lu\n",addr_book_size(ab));
  

  struct addr_book* ab2 = addr_book_create_from_select_name(ab,"Benoist");
  if(NULL == ab2){
    puts("Error: Problem creating the Address book from file.");
    return 1;
  }
  if(0!=addr_book_size(ab2)){
    puts("Error: This address book should be empty");
    return 2;
  }
  addr_book_delete(ab2);

  
  struct addr_book* ab3 = addr_book_create_from_select_name(ab,"Muster");
  if(NULL == ab3){
    puts("Error: Problem creating the Address book from file.");
    return 1;
  }
  fprintf(stdout,"Size created address book = %lu\n",addr_book_size(ab3));
  if(0==addr_book_size(ab3)){
    puts("Error: This address book should not be empty");
    return 2;
  }
  
  addr_book_delete(ab);



  struct addr_book_item* elem1 = addr_book_get_element_at(ab3,1);
    if(NULL == elem1){
      puts("error reading the element with index 1 of new address book");
      return 3;
    }

    int cmp = strcmp(elem1->name,"Muster");
    if(0 != cmp){
      puts("error with names after select");
      printf("Expected name Muster, got %s\n",elem1->name);
      return 5;
      
    }

    addr_book_print(stdout,ab3);
  
  addr_book_delete(ab3);
  return 0;
}


// ==============================================================================


int main(void){

  
    printf("Test 1\n");
    assert(test1()==0);
    printf("Test 2\n");
    assert(test2()==0);
    printf("Test 3\n");
    assert(test3()==0);
    printf("Test 4\n");
    assert(test4()==0);
    printf("Test 5\n");
    assert(test5()==0);

  
}
