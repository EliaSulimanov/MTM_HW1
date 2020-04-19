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

/**
* nodeGetKey: Returns the address of the key for a given node.
*
* @param node - the node to get the key of.
* @return
* 	NULL - if node is NULL.
* 	The key of the node in case of success.
*/
char* nodeGetKey(Node node);

/**
* nodeGetValue: Returns the address of the value for a given node.
*
* @param node - the node to get the value of.
* @return
* 	NULL - if node is NULL.
* 	The value of the node in case of success.
*/
char* nodeGetValue(Node node);

/**
* nodeGetNext: Returns the address of the next node in relation to the given node.
*
* @param node - the node to get the next node of.
* @return
* 	NULL - if node is NULL.
* 	The address of the next node in case of success.
*/
Node nodeGetNext(Node node);

/**
* nodeSetNext: Sets the next node of the current to be a given node.
*
* @param current_node - the node to set the next node of.
* @param next_node - the next node to be setted.
*/
void nodeSetNext(Node current_node, Node next_node);

/**
* nodeSetKey: Sets the key of the given node.
*
* @param node - the node to set the key of.
* @param key - the key to set.
* @return
* 	false - in case of memory error.
* 	ture otherwise.
*/
bool nodeSetKey(Node node, const char* key);

/**
* nodeSetValue: Sets the value of the given node.
*
* @param node - the node to set the value of.
* @param value - the value to set.
* @return
* 	false - in case of memory error.
* 	ture otherwise.
*/
bool nodeSetValue(Node node, const char* value);

/**
* nodeDestroy: Deallocates an existing node list. Clears all elements.
*
* @param node - Target list head to be deallocated. If node is NULL nothing will be
* 		done
*/
void nodeDestroy(Node node);

#endif
