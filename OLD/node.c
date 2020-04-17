#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "node.h"

struct Node_t
{
    char *key;
    char *value;
    Node *next;
};

Node nodeCreate(const char* key, const char* value, Node* next) {
    
    Node node = malloc(sizeof(node));
    if(node == NULL) {
        return NULL;
    }
    
    node->key = malloc(strlen(key) + 1);
    if(node->key == NULL) {
        free(node);
        return NULL;
    }
    strcpy(node->key, key);

    node->value = malloc(strlen(value) + 1);
    if(node->value == NULL) {
        free(node->key); //
        free(node); //
        return NULL;
    }
    strcpy(node->value, value);

    return node;
}

void nodeDestroy(Node node) {
    if(node != NULL) {
        free(node->key);
        free(node->value);
        //free(node->next); //Check if it not breaks the list.
        free(node);
    }
}

void nodeGetKey(Node node, char* key) {
    if(node == NULL) {
        return NULL;
    }
    

    /*char *key = malloc(strlen(node->key) + 1);
    if(key == NULL) {
        return NULL;
    }
    strcpy(key, node->key);
    return node->key;*/
}

void nodeGetValue(Node node, char* value){
    if(node == NULL) {
        return;
    }
    
    value = malloc(strlen(node->value) + 1);
    if(value == NULL) {
        return;
    }

    strcpy(value, node->value);
}

Node nodeGetNext(Node node) {
    if(node == NULL || node->next == NULL) {
        return NULL;
    }
    return *(node->next);
}

bool nodeSetKey(Node node, const char* key){
    assert(node->key != NULL);
    char *temp= malloc(strlen(node->key) + 1);
    if(temp == NULL) {
        return false;
    }
    strcpy(temp, node->key);

    node->key = malloc(strlen(key) + 1);
    if(node->key == NULL) {
        free(temp);
        return false;
    }
    strcpy(node->key,key);
    free(temp);
    return true;
}

bool nodeSetValue(Node node, const char* value){
    assert(node->value != NULL);

    char *temp= malloc(strlen(node->value) + 1);
    if(temp == NULL) {
        return false;
    }
    strcpy(temp, node->value);

    node->value = malloc(strlen(value) + 1);
    if(node->value == NULL) {
        free(temp);
        return false;
    }
    strcpy(node->value, value);
    free(temp);
    return true;
}