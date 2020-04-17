#ifndef NODE_H_
#define NODE_H_

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

Node nodeCreateDummyNode();

char* nodeGetKey(Node node);

Node nodeGetNext(Node node);

void nodeSetNext(Node current_node, Node next_node);

void nodeSetKey(Node node, char* key);

void nodeDestroy(Node node);

#endif
