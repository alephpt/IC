/**
 * parallel.c - Implementation of Parallel Reduction Engine
 */

#include "parallel.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Internal structure for passing data to worker threads
 */
typedef struct {
    ActivePair* pairs;
    size_t count;
    InteractionRule rule;
} ReductionBatch;

/**
 * Worker thread function - applies the interaction rule to all pairs
 * in the assigned batch
 */
static void* reduce_worker(void* arg) {
    ReductionBatch* batch = (ReductionBatch*)arg;
    if (!batch || !batch->pairs || !batch->rule) {
        return NULL;
    }

    for (size_t i = 0; i < batch->count; i++) {
        batch->rule(&batch->pairs[i]);
    }

    return NULL;
}

int parallel_reduce(ActivePair* pairs, size_t pair_count,
                    InteractionRule rule, size_t num_threads) {
    // Validate inputs
    if (!pairs || pair_count == 0 || !rule || num_threads == 0) {
        return -1;
    }

    // Allocate thread structures
    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
    ReductionBatch* batches = malloc(sizeof(ReductionBatch) * num_threads);

    if (!threads || !batches) {
        free(threads);
        free(batches);
        return -1;
    }

    // Calculate pairs per thread (round up to handle uneven distribution)
    size_t pairs_per_thread = (pair_count + num_threads - 1) / num_threads;

    // Create worker threads
    size_t actual_threads = 0;
    for (size_t i = 0; i < num_threads && i * pairs_per_thread < pair_count; i++) {
        batches[i].pairs = &pairs[i * pairs_per_thread];
        batches[i].rule = rule;

        // Handle last thread getting remaining pairs
        size_t start_idx = i * pairs_per_thread;
        size_t end_idx = start_idx + pairs_per_thread;
        batches[i].count = (end_idx > pair_count) ?
                          (pair_count - start_idx) :
                          pairs_per_thread;

        if (pthread_create(&threads[i], NULL, reduce_worker, &batches[i]) != 0) {
            // Thread creation failed, wait for already created threads
            fprintf(stderr, "Error: Failed to create worker thread %zu\n", i);
            for (size_t j = 0; j < i; j++) {
                pthread_join(threads[j], NULL);
            }
            free(threads);
            free(batches);
            return -1;
        }

        actual_threads++;
    }

    // Wait for all threads to complete
    for (size_t i = 0; i < actual_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(batches);
    return 0;
}
