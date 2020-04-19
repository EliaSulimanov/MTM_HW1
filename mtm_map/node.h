#ifndef NODE_H_
#define NODE_H_

#include <stdbool.h>

/** Type for defining the node */
typedef struct Node_t* Node;

/**
* nodeCreate: Allocates a new node.
*
* @param key - key to be added to the node.
* @param value - value to be added to the node.
* @param next - the next node to be pointed by this one.
* @return
* 	NULL - if allocations failed.
* 	A new Node in case of success.
*/
Node nodeCreate(const char* key, const char* value, Node next);

char* nodeGetKey(Node node);

Node nodeGetNext(Node node);

void nodeSetNext(Node current_node, Node next_node);

bool nodeSetKey(Node node, const char* key);

bool nodeSetValue(Node node, const char* value);

void nodeDestroy(Node node);

#endif
