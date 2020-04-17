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
Node nodeCreate(const char* key, const char* value, Node* next);

/**
* nodeDestroy: Deallocates an existing node. Clears all fields.
*
* @param node - Target node to be deallocated. If node is NULL nothing will be
* 		done
*/
void nodeDestroy(Node node);

/**
* nodeGetKey: Returns the key saved on the node
* @param node - The node which key is requested
* @return
* 	NULL if a NULL pointer was sent or memory allocation error occurred.
* 	Otherwise the key of node.
*/
void nodeGetKey(Node node, char* key);

/**
* nodeGetValue: Returns the value saved on the node
* @param node - The node which value is requested
* @param key - pointer for a key to assign the node key to
*/
void nodeGetValue(Node node, char* value);

Node nodeGetNext(Node node);

bool nodeSetKey(Node node, const char* key);

bool nodeSetValue(Node node, const char* value);
#endif