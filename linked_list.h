#ifndef LINKED_LIST
#define LINKED_LIST
/* linked list node definition */

typedef struct struct_of_ints_struct {
    char* value;
    int count;
    struct struct_of_ints_struct *prev ;
    struct struct_of_ints_struct *next ;
} linked_list ;


/* linked list helper function declarations */
linked_list* add_if_absent         (linked_list* head_node, char* string);
linked_list* add                   (linked_list* head_node, char* string);
linked_list* delete_node           (linked_list* head_node, char* string);
linked_list* delete_specific_node  (linked_list* head_node, linked_list* node_to_delete);
linked_list* search_list           (linked_list* head_node, char* string);
linked_list* sort_list             (linked_list* head_node ) ;
linked_list* sort_list_rev         (linked_list* head_node ) ;
void     print_list            (linked_list* head_node ) ;
linked_list* delete_list           (linked_list* head_node ) ;
int      pmain                 ();
int      length                (linked_list* head);
linked_list* get_max (linked_list* node);
linked_list* copy    (linked_list* node);
#endif
