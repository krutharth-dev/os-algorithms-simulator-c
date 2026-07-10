// main.c - Combined OS Algorithms Simulator
#include "include/pagereplacement.h"
#include "include/memoryallocation.h"
#include "include/diskscheduling.h"
#include "include/algorithm_reports.h"
#include "include/ipc.h"

// ========================================
// MENU DISPLAY FUNCTIONS
// ========================================

void displayMainMenu()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║    OPERATING SYSTEM ALGORITHMS SIMULATOR     ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. Page Replacement Algorithms              ║\n");
    printf("║  2. Memory Allocation Algorithms             ║\n");
    printf("║  3. Disk Scheduling Algorithms               ║\n");
    printf("║  4. IPC (Inter-Process Communication)        ║\n");
    printf("║  5. Exit                                     ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

void displayPageReplacementMenu()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║       PAGE REPLACEMENT ALGORITHMS            ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. FIFO (First In First Out)                ║\n");
    printf("║  2. LRU (Least Recently Used)                ║\n");
    printf("║  3. Optimal Page Replacement                 ║\n");
    printf("║  4. LFU (Least Frequently Used)              ║\n");
    printf("║  5. Run All + Comparison Table               ║\n");
    printf("║  6. Back to Main Menu                        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

void displayMemoryAllocationMenu()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║       MEMORY ALLOCATION ALGORITHMS           ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. First Fit                                ║\n");
    printf("║  2. Best Fit                                 ║\n");
    printf("║  3. Worst Fit                                ║\n");
    printf("║  4. Run All                                  ║\n");
    printf("║  5. Back to Main Menu                        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

void displayDiskSchedulingMenu()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║        DISK SCHEDULING ALGORITHMS            ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. FCFS (First Come First Serve)            ║\n");
    printf("║  2. SSTF (Shortest Seek Time First)          ║\n");
    printf("║  3. SCAN (Elevator Algorithm)                ║\n");
    printf("║  4. C-SCAN (Circular SCAN)                   ║\n");
    printf("║  5. LOOK                                     ║\n");
    printf("║  6. C-LOOK                                   ║\n");
    printf("║  7. Run All + Comparison Table               ║\n");
    printf("║  8. Back to Main Menu                        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

void displayIPCMenu()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║   IPC (INTER-PROCESS COMMUNICATION)          ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    printf("║  1. Pipe Communication                       ║\n");
    printf("║  2. Producer-Consumer (Semaphores)           ║\n");
    printf("║  3. Shared Memory - Writer                   ║\n");
    printf("║  4. Shared Memory - Reader                   ║\n");
    printf("║  5. Back to Main Menu                        ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("Enter your choice: ");
}

// ========================================
// MODULE HANDLERS
// ========================================

void handlePageReplacement()
{
    int pages[MAX_PAGES], n, frame_count, choice;

    printf("\n");
    printf("════════════════════════════════════════════════\n");
    printf("        PAGE REPLACEMENT MODULE SETUP           \n");
    printf("════════════════════════════════════════════════\n");

    printf("\nEnter number of frames: ");
    scanf("%d", &frame_count);

    if (frame_count <= 0 || frame_count > MAX_FRAMES)
    {
        printf("❌ Invalid number of frames! Must be 1-%d\n", MAX_FRAMES);
        return;
    }

    printf("Enter number of page references: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_PAGES)
    {
        printf("❌ Invalid number of references! Must be 1-%d\n", MAX_PAGES);
        return;
    }

    printf("Enter page reference string: ");
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &pages[i]);
    }

    while (1)
    {
        displayPageReplacementMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            runFIFO(pages, n, frame_count);
            break;
        case 2:
            runLRU(pages, n, frame_count);
            break;
        case 3:
            runOptimal(pages, n, frame_count);
            break;
        case 4:
            runLFU(pages, n, frame_count);
            break;
        case 5:
            printf("\n════════════════════════════════════════════════\n");
            printf("       RUNNING ALL PAGE REPLACEMENT ALGORITHMS  \n");
            printf("════════════════════════════════════════════════\n");
            runFIFO(pages, n, frame_count);
            runLRU(pages, n, frame_count);
            runOptimal(pages, n, frame_count);
            runLFU(pages, n, frame_count);
            print_page_replacement_comparison(pages, n, frame_count);
            break;
        case 6:
            printf("\n✓ Returning to main menu...\n");
            return;
        default:
            printf("\n❌ Invalid choice! Try again.\n");
        }
    }
}

void handleMemoryAllocation()
{
    int blockSizes[MAX_BLOCKS], processSizes[MAX_PROCESSES];
    int blockCount, processCount, choice;

    printf("\n");
    printf("════════════════════════════════════════════════\n");
    printf("      MEMORY ALLOCATION MODULE SETUP            \n");
    printf("════════════════════════════════════════════════\n");

    printf("\nEnter number of blocks: ");
    scanf("%d", &blockCount);

    if (blockCount <= 0 || blockCount > MAX_BLOCKS)
    {
        printf("❌ Invalid number of blocks! Must be 1-%d\n", MAX_BLOCKS);
        return;
    }

    printf("Enter block sizes:\n");
    for (int i = 0; i < blockCount; i++)
    {
        printf("  Block %d: ", i + 1);
        scanf("%d", &blockSizes[i]);
    }

    printf("\nEnter number of processes: ");
    scanf("%d", &processCount);

    if (processCount <= 0 || processCount > MAX_PROCESSES)
    {
        printf("❌ Invalid number of processes! Must be 1-%d\n", MAX_PROCESSES);
        return;
    }

    printf("Enter process sizes:\n");
    for (int i = 0; i < processCount; i++)
    {
        printf("  Process %d: ", i + 1);
        scanf("%d", &processSizes[i]);
    }

    while (1)
    {
        displayMemoryAllocationMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            allocateMemory(blockSizes, blockCount, processSizes, processCount, 1);
            break;
        case 2:
            allocateMemory(blockSizes, blockCount, processSizes, processCount, 2);
            break;
        case 3:
            allocateMemory(blockSizes, blockCount, processSizes, processCount, 3);
            break;
        case 4:
            printf("\n");
            printf("════════════════════════════════════════════════\n");
            printf("           RUNNING ALL ALGORITHMS               \n");
            printf("════════════════════════════════════════════════\n");
            allocateMemory(blockSizes, blockCount, processSizes, processCount, 1);
            allocateMemory(blockSizes, blockCount, processSizes, processCount, 2);
            allocateMemory(blockSizes, blockCount, processSizes, processCount, 3);
            break;
        case 5:
            printf("\n✓ Returning to main menu...\n");
            return;
        default:
            printf("\n❌ Invalid choice! Try again.\n");
        }
    }
}

void askDiskDirection(int *direction)
{
    printf("\nEnter direction (1 = Right, 0 = Left): ");
    scanf("%d", direction);

    if (*direction != 0 && *direction != 1)
    {
        printf("Invalid direction selected. Defaulting to Right.\n");
        *direction = 1;
    }
}

void handleDiskScheduling()
{
    int requests[MAX_REQUESTS], n, head, direction, choice;

    printf("\n");
    printf("════════════════════════════════════════════════\n");
    printf("       DISK SCHEDULING MODULE SETUP             \n");
    printf("════════════════════════════════════════════════\n");

    printf("\nEnter number of requests: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_REQUESTS)
    {
        printf("❌ Invalid number of requests! Must be 1-%d\n", MAX_REQUESTS);
        return;
    }

    printf("Enter disk requests (0-%d):\n", DISK_SIZE - 1);
    for (int i = 0; i < n; i++)
    {
        printf("  Request %d: ", i + 1);
        scanf("%d", &requests[i]);
        if (requests[i] < 0 || requests[i] >= DISK_SIZE)
        {
            printf("❌ Invalid track number! Must be between 0 and %d\n", DISK_SIZE - 1);
            return;
        }
    }

    printf("\nEnter initial head position (0-%d): ", DISK_SIZE - 1);
    scanf("%d", &head);

    if (head < 0 || head >= DISK_SIZE)
    {
        printf("❌ Invalid head position! Must be between 0 and %d\n", DISK_SIZE - 1);
        return;
    }

    while (1)
    {
        displayDiskSchedulingMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            FCFS(requests, n, head);
            break;
        case 2:
            SSTF(requests, n, head);
            break;
        case 3:
            askDiskDirection(&direction);
            SCAN(requests, n, head, direction);
            break;
        case 4:
            askDiskDirection(&direction);
            CSCAN(requests, n, head, direction);
            break;
        case 5:
            askDiskDirection(&direction);
            LOOK(requests, n, head, direction);
            break;
        case 6:
            askDiskDirection(&direction);
            CLOOK(requests, n, head, direction);
            break;
        case 7:
            askDiskDirection(&direction);
            printf("\n════════════════════════════════════════════════\n");
            printf("       RUNNING ALL DISK SCHEDULING ALGORITHMS   \n");
            printf("════════════════════════════════════════════════\n");
            FCFS(requests, n, head);
            SSTF(requests, n, head);
            SCAN(requests, n, head, direction);
            CSCAN(requests, n, head, direction);
            LOOK(requests, n, head, direction);
            CLOOK(requests, n, head, direction);
            print_disk_scheduling_comparison(requests, n, head, direction);
            break;
        case 8:
            printf("\n✓ Returning to main menu...\n");
            return;
        default:
            printf("\n❌ Invalid choice! Try again.\n");
        }
    }
}

void handleIPC()
{
    int choice;

    printf("\n");
    printf("════════════════════════════════════════════════\n");
    printf("              IPC MODULE ACTIVE                 \n");
    printf("════════════════════════════════════════════════\n");

    while (1)
    {
        displayIPCMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            runPipeIPC();
            break;
        case 2:
            runProducerConsumer();
            break;
        case 3:
            runSharedMemoryWriter();
            break;
        case 4:
            runSharedMemoryReader();
            break;
        case 5:
            printf("\n✓ Returning to main menu...\n");
            return;
        default:
            printf("\n❌ Invalid choice! Try again.\n");
        }
    }
}

// ========================================
// MAIN FUNCTION
// ========================================

int main()
{
    int choice;

    // Welcome Banner
    printf("\n");
    printf("████████████████████████████████████████████████████████\n");
    printf("█                                                      █\n");
    printf("█        OPERATING SYSTEM ALGORITHMS SIMULATOR         █\n");
    printf("█        =====================================         █\n");
    printf("█                                                      █\n");
    printf("█    📄 Page Replacement (FIFO, LRU, Optimal, LFU)     █\n");
    printf("█    💾 Memory Allocation (FF, BF, WF)                 █\n");
    printf("█    💿 Disk Scheduling (FCFS, SSTF, SCAN, C-SCAN)     █\n");
    printf("█       + LOOK and C-LOOK                              █\n");
    printf("█    🔄 IPC (Pipe, Semaphores, Shared Memory)          █\n");
    printf("█                                                      █\n");
    printf("████████████████████████████████████████████████████████\n");

    while (1)
    {
        displayMainMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            handlePageReplacement();
            break;
        case 2:
            handleMemoryAllocation();
            break;
        case 3:
            handleDiskScheduling();
            break;
        case 4:
            handleIPC();
            break;
        case 5:
            printf("\n");
            printf("╔══════════════════════════════════════════════╗\n");
            printf("║                                              ║\n");
            printf("║   Thank you for using OS Algorithms Tool!    ║\n");
            printf("║                                              ║\n");
            printf("║              Program Terminated              ║\n");
            printf("║                                              ║\n");
            printf("║                 Goodbye! 👋                  ║\n");
            printf("║                                              ║\n");
            printf("╚══════════════════════════════════════════════╝\n");
            printf("\n");
            return 0;
        default:
            printf("\n❌ Invalid choice! Please enter 1-5.\n");
        }
    }

    return 0;
}
