// memoryallocation.h
#ifndef MEMORY_ALLOCATION_H
#define MEMORY_ALLOCATION_H

#include <stdio.h>

#define MAX_BLOCKS 10
#define MAX_PROCESSES 10

// ========================================
// MEMORY ALLOCATION ALGORITHMS
// ========================================

void allocateMemory(int blockSizes[], int blockCount, int processSizes[], int processCount, int method)
{
    int available[MAX_BLOCKS];

    // Copy original block sizes for simulation
    for (int i = 0; i < blockCount; i++)
        available[i] = blockSizes[i];

    // Print method header with visual formatting
    if (method == 1)
    {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║           FIRST FIT ALLOCATION         ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\nStrategy: Allocate to first block that fits\n");
    }
    if (method == 2)
    {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║           BEST FIT ALLOCATION          ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\nStrategy: Allocate to smallest block that fits\n");
    }
    if (method == 3)
    {
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║           WORST FIT ALLOCATION         ║\n");
        printf("╚════════════════════════════════════════╝\n");
        printf("\nStrategy: Allocate to largest block available\n");
    }

    printf("\nProcess | Size | Block | Remaining\n");
    printf("----------------------------------------\n");

    int allocated_count = 0;
    int total_internal_fragmentation = 0;

    for (int p = 0; p < processCount; p++)
    {
        int chosenBlock = -1;

        for (int b = 0; b < blockCount; b++)
        {
            if (available[b] >= processSizes[p])
            {
                // First Fit: Pick first available block and break
                if (method == 1)
                {
                    chosenBlock = b;
                    break;
                }

                // Best Fit: Pick smallest available block that fits
                if (method == 2)
                {
                    if (chosenBlock == -1 || available[b] < available[chosenBlock])
                        chosenBlock = b;
                }

                // Worst Fit: Pick largest available block
                if (method == 3)
                {
                    if (chosenBlock == -1 || available[b] > available[chosenBlock])
                        chosenBlock = b;
                }
            }
        }

        if (chosenBlock != -1)
        {
            int remaining = available[chosenBlock] - processSizes[p];
            printf("   P%-2d  | %-4d |   B%-2d  |   %-4d\n",
                   p + 1, processSizes[p], chosenBlock + 1, remaining);

            total_internal_fragmentation += remaining;
            available[chosenBlock] -= processSizes[p];
            allocated_count++;
        }
        else
        {
            printf("   P%-2d  | %-4d | NOT ALLOCATED\n",
                   p + 1, processSizes[p]);
        }
    }

    // Summary statistics
    printf("\n----------------------------------------\n");
    printf("Summary:\n");
    printf("  Allocated: %d/%d processes\n", allocated_count, processCount);
    printf("  Failed: %d processes\n", processCount - allocated_count);
    printf("  Total Internal Fragmentation: %d units\n", total_internal_fragmentation);

    // Calculate external fragmentation
    int external_fragmentation = 0;
    printf("\nRemaining Block Sizes:\n");
    for (int i = 0; i < blockCount; i++)
    {
        if (available[i] > 0)
        {
            printf("  Block %d: %d units\n", i + 1, available[i]);
            external_fragmentation += available[i];
        }
    }
    printf("  Total External Fragmentation: %d units\n", external_fragmentation);
    printf("\n");
}

#endif // MEMORY_ALLOCATION_H