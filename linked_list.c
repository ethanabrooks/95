#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"
#define NAME_SIZE 30

/* definition of linked list helper functions */


linked_list* new_node(char* value) {
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
    new_linked_list->count = 1;
    new_linked_list->next = NULL;
    new_linked_list->prev = NULL;
    return new_linked_list;
}

linked_list* copy(linked_list* node) {
    if (!node) {
        return NULL;
    }
    linked_list* node_copy = new_node(node->value);
    node_copy->count = node->count;
    return node_copy;
}

/* checks for membership before adding
*/
linked_list* add_if_absent (linked_list* head_node, char* value) {
    linked_list* node = head_node;
    for (; node; node = node->next) {
        if (strcmp(node->value, value) == 0) {
            node->count++;
            return head_node;
        }
    }
    return add(head_node, value);
}

/*
 * links node1 to node2 so that node1.next = node2 and node2.prev = node1
 */
linked_list* link_nodes(linked_list* node1, linked_list* node2) {
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
linked_list* add (linked_list* head_node, char* value) {
    linked_list* new_head = new_node(value);
    new_head = link_nodes(new_head, head_node);
    return new_head;
}

/*
 * given a pointer to a node, adds that node to the end of a linked list
 */
linked_list* add_node_to_end
(linked_list* head_node, linked_list* new_node) {
  if (head_node) {
    /* recurse on node after next and link head_node to result */
    link_nodes(head_node, add_node_to_end(head_node->next, new_node));
    return head_node;
  }
  else {  /* if head_node is last node in list, link it to new_node*/
   return link_nodes(head_node, new_node);
  }
}
/*
 * this function adds a node to the end of the linked list
 * -returns head_node - or NULL if error
 */
linked_list* add_to_end (linked_list* head_node, char* name){
  /* define new node with node->value = value */
  linked_list* new_node = malloc (sizeof(linked_list));
  if (new_node == NULL) return NULL ;

  strcpy(new_node->value, name) ;
  new_node->next = NULL;
  new_node->prev = NULL;
   /* add new node to end of list */
  return add_node_to_end(head_node, new_node);
}

/*
 * delete node specified by address (as opposed to value)
 */
linked_list* delete_specific_node
(linked_list* head_node, linked_list* node_to_delete){
    if (!node_to_delete) {
        return NULL;
    }
   /* link nodes adjacent to node_to_delete */
  link_nodes(node_to_delete->prev, node_to_delete->next);
  /* if we delete the head_node, we must return the subsequent node */
    if (node_to_delete == head_node) {
        head_node = head_node->next;
    }
    free(node_to_delete->value);
    free(node_to_delete);
  return head_node;
};

/*
 * deletes the first instance of the "value" from the list
 * -returns head_node - or NULL if node isn't in list
 */
linked_list* delete_node (linked_list* head_node, char* name){
  /* find address of first node with value */
  linked_list* node_to_delete = search_list(head_node, name);
  /* if found, delete it */
  if (node_to_delete) {
    return delete_specific_node(head_node, node_to_delete);
  }
  else return NULL;
}

/*
 * searches list for first instance of the "value" passed in
 * -returns node if found - or a NULL if node isn't in list
 */
linked_list* search_list (linked_list* head_node, char* name){
  if (head_node) {
    /* return address of first node with desired value */
      if (strcmp(head_node->value, name)==0){
      return head_node;
    } else {
      /* recurse through list until first node with value is found */
      return search_list (head_node->next, name);
    }
  } else return NULL;
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
linked_list* sort (linked_list* head, int reverse) {
  if (head) {
    /* before is a list of elements that should come before head */
    linked_list* before = partition(head->next, NULL, head->value, reverse);
    /* after is a list of elements that should come after head */
    linked_list* after = head->next;
    /* recurse on before and after */
    before = sort(before, reverse); after = sort(after, reverse);
    /* create new list: before --> head --> after */
    return add_node_to_end(before, link_nodes(head, after));
  }
  /* if head is the empty list, return NULL */
  else { return head; }
};

/*
 * sorts linked list in acending order: 1, 2, 3...
 * -returns new head of linked list after sort, or NULL if list is empty
 */
linked_list* sort_list (linked_list* head_node ) {
  return sort(head_node, 0); // 0 means "not reversed"
}


/*
 * sorts linked list in decending order: 3, 2, 1...
 * -returns new head of linked list after sort, or NULL if list is empty
 */
linked_list* sort_list_rev (linked_list* head_node ) {
  return sort(head_node, 1) ; // 1 means "reversed"
}

/*
 * aux function to print list that includes linked_list i
 */
void print_list_helper(linked_list* head_node, int i) {
  if (head_node) {
    printf (" %s", head_node->value) ;
    /* recurse on next node */
      if (head_node->next) {
          printf(",");
          print_list_helper(head_node->next, i+1);
      }
  }
};


/*
 * prints entire linked list to look like an array
 */
void print_list (linked_list* head_node ) {
  /* CIT 593 to do: this code only prints the first node,
        print out the rest of the list! */;
  print_list_helper(head_node, 0);
  printf("\n");
};

/*
 * delete every node in the linked list
 * returns NULL if successful, otherwise head node is returned
 */
linked_list* delete_list (linked_list* head_node ) {
  if (head_node) {
      /* if head node exists, delete it */
      linked_list* next = head_node->next;
      free(head_node->value);
      free(head_node);
    if (head_node) {
        head_node = NULL;
    }
    /* recurse on next node */
    return delete_list(next);
  } else { return NULL; }
}

int length(linked_list* head) {
    if (head) {
        return 1 + length(head->next);
    } else return 0;
}

linked_list* get_max(linked_list* head) {
    if (!head) {
        return NULL;
    }
    linked_list* node = head;
    linked_list* max = head;
    for (; node; node = node->next) {
        if (node->count > max->count) {
            max = node;
        }
    }
    return max;
}
