#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

#include "node.h"

struct Node_t
{
    char *key;
    char *value;
    Node *next;
};

Node nodeCreate(const char* key, const char* value, Node* next) {
    
    printf("key: %s, data: %s", key, value); //DEBUGGING

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
        free(node->key);
        free(node);
        return NULL;
    }
    strcpy(node->value, value);

    return node;
}

void nodeDestroy(Node node) {
    assert(node != NULL);
    free(node->key);
    free(node->value);
    free(node->next); //Check if it not breaks the list.
    free(node);
}

char* nodeGetKey(Node node) {
    if(node == NULL) {
        return NULL;
    }

    char *key = malloc(strlen(node->key) + 1);
    if(key == NULL) {
        return NULL;
    }
    strcpy(key, node->key);
    return key;
}

char* nodeGetValue(Node node){
    if(node == NULL) {
        return NULL;
    }
    
    char *value = malloc(strlen(node->value) + 1);
    if(value == NULL) {
        return NULL;
    }
    strcpy(value, node->value);
    return value;
}

Node nodeGetNext(Node node) {
    if(node == NULL) {
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
    strcpy(node->value,value);
    free(temp);
    return true;
}