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
bool isListSorted(Node list) {
    return true;
}

int getListLength(Node list) {
    int list_size = 0;
    Node list_iterator = list;
    while(list_iterator != NULL) {
        list_size++;
        list_iterator = list_iterator->next;
    }
    return list_size;
}

static void printResult(ErrorCode errorCode) {
    switch (errorCode) {
        case 0:
            printf("SUCCESS");
            break;
        case 1:
            printf("MEMORY_ERROR");
            break;
        case 2:
            printf("EMPTY_LIST");
            break;
        case 3:
            printf("UNSORTED_LIST");
            break;
        case 4:
            printf("NULL_ARGUMENT");
            break;
    }
}

//*****************************************************
static void listDestroy(Node list) {
    if (getListLength(list) > 0) {
        if(list == NULL) {
            return;
        }
        Node temp = list;
        while(temp != NULL) {
            list = list->next;
            free(temp);
            temp = list;
        }
    }
}

static ErrorCode copyList(Node list, Node destination) {
    if(list == NULL) {
        return EMPTY_LIST;
    }
    Node list_iterator = list; //Creating iterator for the list to avoid changing the original list.

    while(list_iterator != NULL) {
        destination->x = list_iterator->x;
        if(list_iterator->next == NULL) { //determine the end of the list.
            destination->next = NULL;
        } else {
            destination->next = malloc(sizeof(*(destination->next)));
            if(destination->next == NULL) {
                return MEMORY_ERROR;
            }
            destination = destination->next;
        }
        list_iterator = list_iterator->next;
    }
    return SUCCESS;
}

static ErrorCode checkMergeFunctionArguments(Node list1, Node list2, Node *merged_out) {
    if(list1 == NULL || list2 == NULL) {
        return EMPTY_LIST;
    }

    if(merged_out == NULL) {
        return NULL_ARGUMENT;
    }

    if(!isListSorted(list1) || !isListSorted(list2)) {
        return UNSORTED_LIST;
    }

    return SUCCESS;
}

ErrorCode mergeSortedLists(Node list1, Node list2, Node *merged_out) {
    ErrorCode check_arguments = checkMergeFunctionArguments(list1, list2, merged_out);
    if(check_arguments != SUCCESS) {
        return check_arguments;
    }

    Node merged_out_list = malloc(sizeof(*merged_out_list));
    if(merged_out_list == NULL) {
        merged_out = NULL;
        return MEMORY_ERROR;
    }

    Node merged_out_head = merged_out_list;
    Node first_list_iterator = list1;
    Node second_list_iterator = list2;

    while(first_list_iterator != NULL && second_list_iterator != NULL)
    {
        //assign the smaller value to the merged list node
        merged_out_list->x = first_list_iterator->x < second_list_iterator->x ? first_list_iterator->x : second_list_iterator->x;

        merged_out_list->next = malloc(sizeof(*(merged_out_list->next)));
        if(merged_out_list->next == NULL) {
            //free all the nodes that was allocated to this point
            listDestroy(merged_out_head);
            merged_out = NULL;
            return MEMORY_ERROR;
        }

        merged_out_list = merged_out_list->next;

        if(first_list_iterator->x < second_list_iterator->x) {
            first_list_iterator = first_list_iterator->next;
        } else {
            second_list_iterator = second_list_iterator->next;
        }
    }

    //continue coping the rest of the list that we didnt reached the end of yet
    ErrorCode copy_list = first_list_iterator == NULL ?
            copyList(second_list_iterator, merged_out_list) : copyList(first_list_iterator, merged_out_list);

    if(copy_list == MEMORY_ERROR) {
        listDestroy(merged_out_head);
        merged_out = NULL;
        return MEMORY_ERROR;
    }
    *merged_out = merged_out_head;
    return SUCCESS;
}

//*****************************************************
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
    printResult(result);
    listDestroy(t);
    return 0;
}
