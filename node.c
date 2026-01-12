/**
 * node.c - Implementation of IC Node operations
 */

#include "node.h"
#include <string.h>

Node* create_cons(int label) {
    Node* n = calloc(1, sizeof(Node));
    if (!n) return NULL;

    n->type = CONS;
    n->label = label;
    return n;
}

Node* create_dup(void) {
    Node* n = calloc(1, sizeof(Node));
    if (!n) return NULL;

    n->type = DUP;
    n->label = 0;
    return n;
}

Node* create_erase(void) {
    Node* n = calloc(1, sizeof(Node));
    if (!n) return NULL;

    n->type = ERASE;
    n->label = 0;
    return n;
}

void free_node(Node* node) {
    if (node) {
        free(node);
    }
}

const char* node_type_str(NodeType type) {
    switch (type) {
        case CONS: return "γ";
        case DUP: return "δ";
        case ERASE: return "ε";
        default: return "?";
    }
}
