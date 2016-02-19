#ifndef HASH_TABLE_H2
#define HASH_TABLE_H2

#include "linked_list.h"

/* hash table structure and helper function declarations */

typedef struct hash_table_struct {

	int num_of_buckets ;		/* size of hash table     */
	linked_list **table ;	/* array of head pointers */

} hash_table ;


/* Hash Table helper function declarations */
hash_table* 	create_hash_table (int num_of_buckets) ;
hash_table* 	double_table_size (hash_table* table, int num_of_buckets) ;
linked_list*	    find			  (hash_table* table, char* hashtag);
unsigned int   	map               (hash_table* table, char* name);
int            	delete_from_table (hash_table* table, char* name) ;
int            	add_to_table      (hash_table* table, char* name);
void           	print_table       (hash_table* table) ;
void           	delete_table      (hash_table* table) ;
linked_list*        extract_max       (hash_table* table);
linked_list*    to_linked_list(hash_table* table);

#endif
