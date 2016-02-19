#ifndef LINKED_LIST
#define LINKED_LIST
/* linked list node definition */

typedef struct struct_of_ints_struct {
    char *value;
    struct struct_of_ints_struct *prev ;
    struct struct_of_ints_struct *next ;
} linked_list ;


/* linked list helper function declarations */
int add_if_absent         (linked_list **list, char *string);
int add                   (linked_list **list, char *string);
int delete_node           (linked_list **list, char *string);
int delete_specific_node  (linked_list **list, linked_list *node_to_delete);
linked_list *search_list  (linked_list *head_node, char *string);
int sort_list             (linked_list **list ) ;
int sort_list_rev         (linked_list **list ) ;
void delete_list          (linked_list **list ) ;
int length                (linked_list *head);
void print_list           (linked_list *head_node ) ;
linked_list *copy         (linked_list *node);
#endif
