/**
 * rules.c - Implementation of IC Interaction Rules
 */

#include "rules.h"
#include <stdio.h>
#include <pthread.h>

void interact_cons_cons_same(ActivePair* pair) {
    if (!pair || !pair->left || !pair->right) return;

    printf("  [Thread %lu] Rule: γ(%d)-γ(%d) → annihilate\n",
           pthread_self(), pair->left->label, pair->right->label);

    // In a full implementation, we would connect the auxiliary ports
    // For this minimal demo, we just free the nodes
    if (pair->left->ports[1] && pair->right->ports[1]) {
        pair->left->ports[1]->ports[0] = pair->right->ports[1];
        pair->right->ports[1]->ports[0] = pair->left->ports[1];
    }
    if (pair->left->ports[2] && pair->right->ports[2]) {
        pair->left->ports[2]->ports[0] = pair->right->ports[2];
        pair->right->ports[2]->ports[0] = pair->left->ports[2];
    }

    free_node(pair->left);
    free_node(pair->right);
}

void interact_dup_cons(ActivePair* pair) {
    if (!pair || !pair->left || !pair->right) return;

    printf("  [Thread %lu] Rule: δ-γ(%d) → duplicate constructor\n",
           pthread_self(), pair->right->label);

    // Create two copies of the constructor
    Node* cons1 = create_cons(pair->right->label);
    Node* cons2 = create_cons(pair->right->label);

    // Create two new duplicators for the auxiliary ports
    Node* dup1 = create_dup();
    Node* dup2 = create_dup();

    // Wire up the connections (simplified)
    if (cons1 && dup1) {
        cons1->ports[1] = dup1;
    }
    if (cons2 && dup2) {
        cons2->ports[1] = dup2;
    }

    free_node(pair->left);
    free_node(pair->right);
}

void interact_erase_cons(ActivePair* pair) {
    if (!pair || !pair->left || !pair->right) return;

    printf("  [Thread %lu] Rule: ε-γ(%d) → erase constructor\n",
           pthread_self(), pair->right->label);

    // Create erasers for the auxiliary ports
    if (pair->right->ports[1]) {
        Node* e1 = create_erase();
        if (e1) {
            e1->ports[0] = pair->right->ports[1];
        }
    }
    if (pair->right->ports[2]) {
        Node* e2 = create_erase();
        if (e2) {
            e2->ports[0] = pair->right->ports[2];
        }
    }

    free_node(pair->left);
    free_node(pair->right);
}

void interact_dup_dup(ActivePair* pair) {
    if (!pair || !pair->left || !pair->right) return;

    printf("  [Thread %lu] Rule: δ-δ → duplicate duplicator\n",
           pthread_self());

    // Create four new duplicators in a specific configuration
    Node* dup1 = create_dup();
    Node* dup2 = create_dup();
    Node* dup3 = create_dup();
    Node* dup4 = create_dup();

    // Wire them up (simplified)
    if (dup1 && dup2) {
        dup1->ports[0] = dup2;
        dup2->ports[0] = dup1;
    }
    if (dup3 && dup4) {
        dup3->ports[0] = dup4;
        dup4->ports[0] = dup3;
    }

    free_node(pair->left);
    free_node(pair->right);
}

void interact_erase_dup(ActivePair* pair) {
    if (!pair || !pair->left || !pair->right) return;

    printf("  [Thread %lu] Rule: ε-δ → erase duplicator\n",
           pthread_self());

    // Create erasers for the auxiliary ports
    Node* e1 = create_erase();
    Node* e2 = create_erase();

    if (e1 && pair->right->ports[1]) {
        e1->ports[0] = pair->right->ports[1];
    }
    if (e2 && pair->right->ports[2]) {
        e2->ports[0] = pair->right->ports[2];
    }

    free_node(pair->left);
    free_node(pair->right);
}
