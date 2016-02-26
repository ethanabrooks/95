#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#define NAME_SIZE 30

/* definition of linked list helper functions */


linked_list* create_node(char* value) {
    linked_list* new_linked_list = calloc(sizeof(linked_list), 1);
    if (!new_linked_list) {
        perror("calloc failed");
        exit(-1);
    }
    new_linked_list->value = calloc(strlen(value) + 1, sizeof(char));
    if (!new_linked_list->value) {
        perror("calloc failed");
        exit(-1);
    }
    strcpy(new_linked_list->value, value);
    new_linked_list->next = NULL;
    new_linked_list->prev = NULL;
    return new_linked_list;
}

linked_list* copy(linked_list* node) {
    if (!node) {
        return NULL;
    }
    linked_list* node_copy = create_node(node->value);
    node_copy->next = copy(node->next);
    return node_copy;
}

/* checks for membership before adding
*/
int add_if_absent (linked_list** list, char* value) {
    linked_list *node = *list;
    for (; node; node = node->next) {
        if (strcmp(node->value, value) == 0) {
            return 0;
        }
    }
    return add(list, value);
}

/*
 * links node1 to node2 so that node1.next = node2 and node2.prev = node1
 */
linked_list* link_nodes(linked_list *node1, linked_list *node2) {
  if (node2){
    node2->prev = node1;
  }
  if (node1) {
    node1->next = node2;
    return node1;
  }
  else return node2;
}

/*
 * this function adds a node to the beginning of the linked list
 * -returns head_node - or NULL if error
*/
int add (linked_list **list, char* value) {
  if (!list) {
    return -1;
  }
  linked_list *new_head = create_node(value);
  *list = link_nodes(new_head, *list);
  return 0;
}

/*
 * given a pointer to a node, adds that node to the end of a linked list
 */
int add_node_to_end(linked_list **list, linked_list *new_node) {
  if (!list) {
    return -1;
  } 
  linked_list *node = *list;
  while (node) { 
    node = node->next;
  }
  link_nodes(node, new_node);
  return 0;
}
/*
 * this function adds a node to the end of the linked list
 * -returns head_node - or NULL if error
 */
int add_to_end (linked_list **list, char* name){
  /* define new node with node->value = value */
  linked_list* new_node = create_node(name);
   /* add new node to end of list */
  return add_node_to_end(list, new_node);
}

/*
 * delete node specified by address (as opposed to value)
 */
int delete_specific_node (linked_list** list, linked_list* node_to_delete){
  if (!node_to_delete) {
      return -1;
  }
  /* link nodes adjacent to node_to_delete */
  link_nodes(node_to_delete->prev, node_to_delete->next);
  /* if we delete the head_node, we must return the subsequent node */
  if (node_to_delete == *list) {
      *list = (*list)->next;
  }
  free(node_to_delete->value);
  free(node_to_delete);
  return 0;
};

/*
 * deletes the first instance of the "value" from the list
 * -returns head_node - or NULL if node isn't in list
 */
int delete_node (linked_list **list, char* name){
  /* find address of first node with value */
  linked_list *node_to_delete = search_list(*list, name);
  /* if found, delete it */
  if (node_to_delete) {
    return delete_specific_node(list, node_to_delete);
  }
  else return -1;
}

/*
 * searches list for first instance of the "value" passed in
 * -returns node if found - or a NULL if node isn't in list
 */
linked_list *search_list (linked_list *head_node, char* value){
  for (; head_node; head_node = head_node->next) {
    if (strcmp(head_node->value, value)) {
      return head_node;
    }
  }
  return NULL;
}


/*
 * Returns head of linked list with all values less than pivot before
 * all other values;
 *
 * @node: node to compare with pivot value
 * @head: address of first node (changes with each recursion);
 * @pivot: value to compare node->value with;
 * @reverse: boolean indicating whether node is being sorted in reverse order
 */
linked_list* partition
(linked_list* node, linked_list* head, char* pivot, int reverse){
  if (node) {
    linked_list* next = node->next;
      int node_comes_before_pivot = strcmp(head->value, pivot) < 0;
      if (reverse)
          node_comes_before_pivot = !node_comes_before_pivot;

    if (node_comes_before_pivot) {
        /* put node at beginning of list */
        link_nodes(node->prev, next);
        link_nodes(node, head);
        head = node;
    };
    /* recurse on next node in list */
    return partition(next, head, pivot, reverse);
  }
  /* at this point, head is the first element in the list */
  else { return head; }
}

/*
 * quicksort
 */
int sort (linked_list** list, int reverse) {
  if (list) {
    linked_list *head = *list;
    /* before is a list of elements that should come before head */
    linked_list* before = partition(head->next, NULL, head->value, reverse);
    /* after is a list of elements that should come after head */
    linked_list* after = head->next;
    /* recurse on before and after */
    sort(&before, reverse); sort(&after, reverse);
    /* create new list: before --> head --> after */
    return add_node_to_end(&before, link_nodes(head, after));
  }
  /* if head is the empty list, return NULL */
  else { return 0; }
};

/*
 * sorts linked list in acending order: 1, 2, 3...
 * -returns new head of linked list after sort, or NULL if list is empty
 */
int sort_list (linked_list** list ) {
  return sort(list, 0); // 0 means "not reversed"
}


/*
 * sorts linked list in decending order: 3, 2, 1...
 * -returns new head of linked list after sort, or NULL if list is empty
 */
int sort_list_rev (linked_list** list ) {
  return sort(list, 1) ; // 1 means "reversed"
}


/*
 * prints entire linked list to look like an array
 */
void print_list (linked_list* head_node ) {
  /* CIT 593 to do: this code only prints the first node,
        print out the rest of the list! */;

  printf ("%s", head_node->value) ;
  head_node = head_node->next;
  for (; head_node; head_node = head_node->next) {
    printf (", %s", head_node->value) ;
  }
};

/*
 * delete every node in the linked list
 * returns NULL if successful, otherwise head node is returned
 */
void delete_list_helper (linked_list* head_node) {
  if (head_node != NULL) {
      /* if head node exists, delete it */
      linked_list *next = head_node->next;
      printf("DELETING %s\n", head_node->value);
      free(head_node->value);
      free(head_node);
    /* recurse on next node */
      delete_list_helper(next); 
  }
}

void delete_list (linked_list **list) {
  delete_list_helper(*list);
  *list = NULL;
}

int length(linked_list* head) {
  int length = 0;
  for (; head; head = head->next) {
    length++;
  }
  return length;
}

/*linked_list* get_max(linked_list* head) {*/
  /*if (!head) {*/
    /*return NULL;*/
  /*}*/
  /*linked_list* node = head;*/
  /*linked_list* max = head;*/
  /*for (; node; node = node->next) {*/
    /*if (node->count > max->count) {*/
        /*max = node;*/
    /*}*/
  /*}*/
  /*return max;*/
/*}*/
