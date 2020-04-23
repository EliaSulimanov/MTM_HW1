#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "node.h"

struct Node_t
{
    char *key;
    char *value;
    Node next;
};

Node nodeCreate(const char* key, const char* value, Node next) {
    Node node = malloc(sizeof(*node));
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

    node->next = next;

    return node;
}

char* nodeGetKey(Node node) {
    if(node == NULL) {
        return NULL;
    }
    return node->key;
}

char* nodeGetValue(Node node) {
    if(node == NULL) {
        return NULL;
    }
    return node->value;
}

bool nodeSetValue(Node node, const char* value) {
    free(node->value);
    node->value = malloc(strlen(value) + 1);
    if(node->value == NULL) {
        return false;
    }
    strcpy(node->value, value);
    return true;
}

Node nodeGetNext(Node node) {
    if(node == NULL) {
        return NULL;
    }

    return node->next;
}

void nodeSetNext(Node current_node, Node next_node) {
    if(current_node == NULL) {
        return;
    }
    current_node->next = next_node;
}

void nodeDestroy(Node node) {
    if(node == NULL) {
        return;
    }

    free(node->key);
    free(node->value);
    node->next = NULL;
    free(node);
}
