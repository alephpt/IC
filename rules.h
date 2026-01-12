/**
 * rules.h - Interaction Combinator Rules
 *
 * Defines the interaction rules that govern how nodes interact
 * when connected at their principal ports (forming "active pairs").
 *
 * Key IC Rules:
 * 1. γ-γ annihilation: Same label constructors cancel out
 * 2. δ-γ duplication: Duplicator copies constructor
 * 3. ε-γ erasure: Eraser removes constructor
 * 4. δ-δ duplication: Duplicator copies duplicator
 * 5. ε-δ erasure: Eraser removes duplicator
 *
 * These rules are:
 * - Local: Only involve connected nodes
 * - Confluent: Order of application doesn't matter
 * - Parallel: Independent rules can fire simultaneously
 */

#ifndef IC_RULES_H
#define IC_RULES_H

#include "node.h"

/**
 * Active pair - two nodes connected at principal ports
 * This is where interactions occur in IC
 */
typedef struct {
    Node* left;
    Node* right;
} ActivePair;

/**
 * Function type for interaction rules
 * Takes an active pair and performs the appropriate rewrite
 */
typedef void (*InteractionRule)(ActivePair* pair);

/**
 * γ-γ annihilation (same label)
 * When two constructors with the same label meet, they cancel out
 * and their auxiliary ports are connected directly.
 */
void interact_cons_cons_same(ActivePair* pair);

/**
 * δ-γ duplication
 * When a duplicator meets a constructor, it creates two copies
 * of the constructor, each with its own duplicator on the auxiliary ports.
 */
void interact_dup_cons(ActivePair* pair);

/**
 * ε-γ erasure
 * When an eraser meets a constructor, the constructor is removed
 * and erasers are placed on its auxiliary ports.
 */
void interact_erase_cons(ActivePair* pair);

/**
 * δ-δ duplication
 * When two duplicators meet, they create a configuration that
 * preserves the duplication behavior.
 */
void interact_dup_dup(ActivePair* pair);

/**
 * ε-δ erasure
 * When an eraser meets a duplicator, the duplicator is removed
 * and erasers are placed on its auxiliary ports.
 */
void interact_erase_dup(ActivePair* pair);

#endif // IC_RULES_H
