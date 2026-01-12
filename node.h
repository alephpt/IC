/**
 * node.h - Interaction Combinators Node Definitions
 *
 * Defines the three fundamental node types in IC:
 * - Constructor (γ): Has a label, used for data structures
 * - Duplicator (δ): Copies information flowing through it
 * - Eraser (ε): Discards information
 *
 * Each node has three ports:
 * - Port 0: Principal port (where interactions happen)
 * - Ports 1-2: Auxiliary ports (connect to other nodes)
 */

#ifndef IC_NODE_H
#define IC_NODE_H

#include <stdlib.h>

/**
 * Three fundamental node types in Interaction Combinators
 */
typedef enum {
    CONS,   // Constructor (γ) - labeled node for data
    DUP,    // Duplicator (δ) - copies data
    ERASE   // Eraser (ε) - discards data
} NodeType;

/**
 * IC Node structure
 *
 * Represents a node in the interaction net with:
 * - type: Which kind of node (CONS, DUP, ERASE)
 * - label: For constructors, distinguishes different types
 * - ports: Connections to other nodes [principal, aux1, aux2]
 */
typedef struct Node {
    NodeType type;
    int label;
    struct Node* ports[3];
} Node;

/**
 * Create a new Constructor node with the given label
 */
Node* create_cons(int label);

/**
 * Create a new Duplicator node
 */
Node* create_dup(void);

/**
 * Create a new Eraser node
 */
Node* create_erase(void);

/**
 * Free a node and its resources
 */
void free_node(Node* node);

/**
 * Get a string representation of a node type (for debugging)
 */
const char* node_type_str(NodeType type);

#endif // IC_NODE_H
