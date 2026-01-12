#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IC Node types
typedef enum { CONS, DUP, ERASE } NodeType;

// IC Node structure
typedef struct Node {
    NodeType type;
    int label;           // For constructors
    struct Node* ports[3]; // ports[0] = principal, ports[1-2] = auxiliary
} Node;

// Active pair (two nodes connected at principal ports)
typedef struct {
    Node* left;
    Node* right;
} ActivePair;

// Create nodes
Node* create_cons(int label) {
    Node* n = calloc(1, sizeof(Node));
    n->type = CONS;
    n->label = label;
    return n;
}

Node* create_dup() {
    Node* n = calloc(1, sizeof(Node));
    n->type = DUP;
    return n;
}

Node* create_erase() {
    Node* n = calloc(1, sizeof(Node));
    n->type = ERASE;
    return n;
}

// IC Interaction Rules

// γ-γ annihilation (same label)
void interact_cons_cons_same(ActivePair* pair) {
    printf("  [Thread %lu] Rule: γ(%d)-γ(%d) annihilate\n",
           pthread_self(), pair->left->label, pair->right->label);

    // Connect auxiliary ports directly
    if (pair->left->ports[1] && pair->right->ports[1]) {
        pair->left->ports[1]->ports[0] = pair->right->ports[1];
        pair->right->ports[1]->ports[0] = pair->left->ports[1];
    }
    if (pair->left->ports[2] && pair->right->ports[2]) {
        pair->left->ports[2]->ports[0] = pair->right->ports[2];
        pair->right->ports[2]->ports[0] = pair->left->ports[2];
    }

    free(pair->left);
    free(pair->right);
}

// δ-γ duplication
void interact_dup_cons(ActivePair* pair) {
    printf("  [Thread %lu] Rule: δ-γ(%d) duplicate\n",
           pthread_self(), pair->right->label);

    // Create two copies of constructor
    Node* cons1 = create_cons(pair->right->label);
    Node* cons2 = create_cons(pair->right->label);

    // Create two new duplicators
    Node* dup1 = create_dup();
    Node* dup2 = create_dup();

    // Rewire connections (simplified)
    cons1->ports[1] = dup1;
    cons2->ports[1] = dup2;

    free(pair->left);
    free(pair->right);
}

// ε-γ erasure
void interact_erase_cons(ActivePair* pair) {
    printf("  [Thread %lu] Rule: ε-γ(%d) erase\n",
           pthread_self(), pair->right->label);

    // Create erasers for auxiliary ports
    if (pair->right->ports[1]) {
        Node* e1 = create_erase();
        e1->ports[0] = pair->right->ports[1];
    }
    if (pair->right->ports[2]) {
        Node* e2 = create_erase();
        e2->ports[0] = pair->right->ports[2];
    }

    free(pair->left);
    free(pair->right);
}

// Parallel reduction engine
typedef void (*InteractionRule)(ActivePair*);

typedef struct {
    ActivePair* pairs;
    size_t count;
    InteractionRule rule;
} ReductionBatch;

void* reduce_worker(void* arg) {
    ReductionBatch* batch = (ReductionBatch*)arg;
    for (size_t i = 0; i < batch->count; i++) {
        batch->rule(&batch->pairs[i]);
    }
    return NULL;
}

void parallel_reduce(ActivePair* pairs, size_t pair_count,
                     InteractionRule rule, size_t num_threads) {
    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
    ReductionBatch* batches = malloc(sizeof(ReductionBatch) * num_threads);

    size_t pairs_per_thread = pair_count / num_threads;

    for (size_t i = 0; i < num_threads; i++) {
        batches[i].pairs = &pairs[i * pairs_per_thread];
        batches[i].count = (i == num_threads - 1) ?
                           (pair_count - i * pairs_per_thread) :
                           pairs_per_thread;
        batches[i].rule = rule;

        pthread_create(&threads[i], NULL, reduce_worker, &batches[i]);
    }

    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(batches);
}

// Demo
int main() {
    printf("=== IC Parallel Reduction Demo ===\n\n");

    // Create some active pairs
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

    printf("Reducing annihilation pairs in parallel...\n");
    parallel_reduce(pairs, 2, interact_cons_cons_same, 2);

    printf("\nReducing duplication pair...\n");
    parallel_reduce(&pairs[2], 1, interact_dup_cons, 1);

    printf("\nReducing erasure pair...\n");
    parallel_reduce(&pairs[3], 1, interact_erase_cons, 1);

    printf("\n=== Demo Complete ===\n");
    return 0;
}
