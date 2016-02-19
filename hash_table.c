#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#include "hash_table.h"

/* definition of hash table helper functions */


/*
 * this function creates a hash table, allocating memory on the heap for it
 * -returns pointer to the hash_table - or NULL if error
 */
hash_table* create_hash_table (int num_of_buckets)
{
//  int i ;

  if (num_of_buckets<1) return NULL; /* invalid size for table */
  /* check if "table" isn't NULL, don't want to destroy an existing table */
  /* allocate memory for 1 hashtable structure */
  hash_table *table = calloc(1, sizeof(hash_table));
  /* allocate memory for each bucket*/
  table->table = calloc(num_of_buckets, sizeof(linked_list*));
  /* set the table's size */
  table->num_of_buckets = num_of_buckets;
  return table;
}


/*
 * this function allows the user to double the # of buckets at run time
 * -returns pointer to the hash_table - or NULL if error
 */
hash_table* double_table_size (hash_table* table, int num_of_buckets)
{

  /* allocate memory for a new hash table - double the size */

  /* students may use "realloc()" if they understand it properly! - not required */
    hash_table* new_table = create_hash_table(num_of_buckets * 2);
    if (new_table) {
    
        int i;
        for (i=0; i<table->num_of_buckets; i++) {
            while (table->table[i]) {
                char * name = table->table[i]->value;
                add_to_table(new_table, name);
                delete_from_table(table, name);
            }
        }
        delete_table(table);
        return new_table;
    } else {
        return NULL;
    }

  /* move data from old hash table to new hash table - if necessary */

  /* don't forget to free old hash table memory before returning */

}


/*
 * determines which "bucket" the value maps to
 * -returns bucket #
 */
unsigned int map(hash_table* table, char* name)
{
//  unsigned int bucket ;

  /* apply hashing function to value that comes in, returning matching bucket # */
    unsigned int result = *name - 'A';
    return result%table->num_of_buckets;
}

/*
 * adds a value to the proper bucket in the hash table
 * -returns 0 upon success
 */
int add_to_table (hash_table* table, char* value)
{
    if (table) {
      /* determine proper bucket to put value into */
        unsigned int bucket_num = map(table, value);
        linked_list** bucket_ptr = &table->table[bucket_num] ;

      /* add value to proper bucket */
        *bucket_ptr = add_if_absent(*bucket_ptr, value);
    }
    return 0 ;
}


/*
 * deletes first instance of value from the hash table
 * -returns 0 upon success
 */
int delete_from_table (hash_table* table, char* value)
{

  /* determine proper bucket value is in */
    linked_list* search_result = find(table, value);

  /* delete data from bucket's linked list */
    if (search_result) {
        int bucket_num = map(table, value);
        linked_list* bucket = delete_node(table->table[bucket_num], value);
        table->table[bucket_num] = bucket;
    }

  return 0 ;

}


/*
 * searches hash table for first instance of "value"
 * -returns actual struct_of_ints node when found, NULL if not found
 */
linked_list* find (hash_table* table, char* value) {

    if (table) {
      /* determine bucket value may be in */
        int bucket_num = map(table, value);

      /* search corresponding bucket's linked list (using search_list function) */


      /* return matching node or NULL if not found */

      return search_list(table->table[bucket_num], value);
    } else return NULL;
}


/*
 * frees all memory originally allocated to the hash table
 */
void delete_table (hash_table* table)
{
    if (table) {
      /* free memory reserved for hash table and linked lists */
        int i;
        for (i=0; i<table->num_of_buckets; i++) {
            table->table[i] = delete_list(table->table[i]);
        }
        free(table->table);
    }
    free(table);
}


/*
 * prints out hash table as if it were 2D array
 */
void print_table (hash_table* table) {

  /* print table as if it were a 2D array:

     bucket[0]={ a, b, c } 
           ...
     bucket[n]={ x, y, z } 
  */
    if (table) {
        for (int i=0; i<table->num_of_buckets; i++) {
            if (table->table[i]) {
                printf("bucket[%d]={", i);
                print_list(table->table[i]);
                printf(" }\n");
            }
        }
    }
}

linked_list* extract_max (hash_table* table) {
    linked_list* global_max = NULL;
    linked_list** max_bucket_ptr = NULL;
    
    //retrieve max counts from each bucket
    for (int i = 0; i < table->num_of_buckets; i++) {
        linked_list* local_max = get_max(table->table[i]);
        
        // update global max
        int new_max = global_max && local_max &&
            (local_max->count > global_max->count);
        if (global_max == NULL || new_max) {
            global_max = local_max;
            max_bucket_ptr = &table->table[i];
        }
    }
    linked_list* max_copy = copy(global_max);
    *max_bucket_ptr = delete_specific_node(*max_bucket_ptr, global_max);
    return max_copy;
}

linked_list* to_linked_list(hash_table* table) {
  linked_list* new_list = NULL;
  for (int i = 0; i < table->num_of_buckets; i++) {
    linked_list* bucket = table->table[i];
    for (; bucket; bucket = bucket->next) {
      new_list = add(new_list, bucket->value);
    }
  }
  return new_list;
}
