#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t {
    int x;  
    struct node_t *next;  
} *Node;     
      
typedef enum {
    SUCCESS=0,  
    MEMORY_ERROR,  
    EMPTY_LIST,  
    UNSORTED_LIST,  
    NULL_ARGUMENT,  
} ErrorCode;

int getListLength(Node list);
bool isListSorted(Node list);
ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out);

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out) {
    if(list1 == NULL || list2 == NULL) {
        return EMPTY_LIST;
    }

    if(merged_out == NULL) {
        return NULL_ARGUMENT;
    }
    
    /*if(!isListSorted(list1) || !isListSorted(list2)) {
        return UNSORTED_LIST;
    }*/

    while(list1 != NULL && list2 != NULL) {
        if(list1->x < list2->x) {
            *merged_out = list1;
            list1 = list1->next;
        } else {
            *merged_out = list2;
            list2 = list2->next;
        }
        merged_out = &((*merged_out)->next);
    }
    *merged_out = list1 ? list1 : list2; //Check what list is the one that ended and tail the other one
    
    return SUCCESS;
}

struct node_t l1[] = {
  { 1, &l1[1] },
  { 3, &l1[2] },
  { 5, &l1[3] },
  { 7, &l1[4] },
  { 9, &l1[5] },
  {11, 0 },
};

struct node_t l2[] = {
  { 2, &l2[1] },
  { 4, &l2[2] },
  { 6, 0 },
};

int main(void) {
    Node t = NULL;
    ErrorCode result = mergeSortedLists(l1, l2, &t);
    for(; t; t = t->next) 
        printf("%d\n", t->x);
    printf("result: %d", result);
    Node l11 = l1;
    for(; l11; l11 = l11->next) 
        printf("%d\n", l11->x);
    return 0;
}