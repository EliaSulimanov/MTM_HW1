#ifndef NODE_H_
#define NODE_H_

#include <stdbool.h>

/** Type for defining the node */
typedef struct Node_t* Node;

Node nodeCreate(const char* key, const char* value, Node* next);

void nodeDestroy(Node node);

char* nodeGetKey(Node node);

char* nodeGetValue(Node node);

Node nodeGetNext(Node node);

bool nodeSetKey(Node node, const char* key);

bool nodeSetValue(Node node, const char* value);
#endif