/**
 * parallel.h - Parallel Reduction Engine for IC
 *
 * Provides parallel execution of IC interaction rules on active pairs.
 * This demonstrates one of IC's key properties: confluence, which means
 * independent reductions can happen in any order (including parallel)
 * without affecting the final result.
 *
 * The parallel reduction engine:
 * 1. Takes a collection of active pairs
 * 2. Distributes them across worker threads
 * 3. Applies interaction rules in parallel
 * 4. Waits for all reductions to complete
 */

#ifndef IC_PARALLEL_H
#define IC_PARALLEL_H

#include "rules.h"
#include <stddef.h>

/**
 * Perform parallel reduction on a batch of active pairs
 *
 * @param pairs Array of active pairs to reduce
 * @param pair_count Number of pairs in the array
 * @param rule The interaction rule to apply to each pair
 * @param num_threads Number of worker threads to use
 *
 * @return 0 on success, -1 on error
 *
 * Note: All pairs should be independent (no shared nodes) to avoid
 * race conditions. This is guaranteed by IC's locality property.
 */
int parallel_reduce(ActivePair* pairs, size_t pair_count,
                    InteractionRule rule, size_t num_threads);

#endif // IC_PARALLEL_H
