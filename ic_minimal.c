/**
 * ic_minimal.c - Simple Demonstration of Interaction Combinators
 *
 * This program demonstrates the basic concepts of IC:
 * - Three node types (Constructor, Duplicator, Eraser)
 * - Interaction rules (annihilation, duplication, erasure)
 * - Parallel reduction of independent active pairs
 *
 * Compile: make ic_minimal
 * Run: ./ic_minimal
 */

#include "node.h"
#include "rules.h"
#include "parallel.h"
#include <stdio.h>

int main(void) {
    printf("╔══════════════════════════════════════════╗\n");
    printf("║  IC Parallel Reduction Demo             ║\n");
    printf("╚══════════════════════════════════════════╝\n\n");

    // Create some active pairs to demonstrate IC rules
    ActivePair pairs[4];

    // Pair 1: γ(1)-γ(1) annihilation
    pairs[0].left = create_cons(1);
    pairs[0].right = create_cons(1);

    // Pair 2: γ(2)-γ(2) annihilation
    pairs[1].left = create_cons(2);
    pairs[1].right = create_cons(2);

    // Pair 3: δ-γ(3) duplication
    pairs[2].left = create_dup();
    pairs[2].right = create_cons(3);

    // Pair 4: ε-γ(4) erasure
    pairs[3].left = create_erase();
    pairs[3].right = create_cons(4);

    // Demonstrate parallel reduction
    printf("1. Reducing annihilation pairs in parallel...\n");
    if (parallel_reduce(pairs, 2, interact_cons_cons_same, 2) != 0) {
        fprintf(stderr, "Error in parallel reduction\n");
        return 1;
    }

    printf("\n2. Reducing duplication pair...\n");
    if (parallel_reduce(&pairs[2], 1, interact_dup_cons, 1) != 0) {
        fprintf(stderr, "Error in duplication\n");
        return 1;
    }

    printf("\n3. Reducing erasure pair...\n");
    if (parallel_reduce(&pairs[3], 1, interact_erase_cons, 1) != 0) {
        fprintf(stderr, "Error in erasure\n");
        return 1;
    }

    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║  Demo Complete                           ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("\nKey Observations:\n");
    printf("  • Different thread IDs show parallel execution\n");
    printf("  • Independent pairs reduce without interference\n");
    printf("  • Each rule is local (only touches connected nodes)\n");

    return 0;
}
