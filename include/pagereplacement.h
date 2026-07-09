#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 10
#define MAX_PAGES 100

// ========================================
// LRU DOUBLY LINKED LIST STRUCTURES
// ========================================

typedef struct Node
{
    int page;
    struct Node *prev, *next;
} Node;

Node *createNode(int page)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->page = page;
    newNode->prev = newNode->next = NULL;
    return newNode;
}

void moveToHead(Node **head, Node **tail, Node *node)
{
    if (*head == node)
        return;

    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    if (*tail == node)
        *tail = node->prev;

    node->next = *head;
    node->prev = NULL;

    if (*head)
        (*head)->prev = node;

    *head = node;
}

void insertAtHead(Node **head, Node **tail, int page)
{
    Node *newNode = createNode(page);
    if (*head == NULL)
    {
        *head = *tail = newNode;
        return;
    }

    newNode->next = *head;
    (*head)->prev = newNode;
    *head = newNode;
}

void removeTail(Node **head, Node **tail)
{
    if (*tail == NULL)
        return;

    Node *temp = *tail;
    if ((*tail)->prev)
    {
        *tail = (*tail)->prev;
        (*tail)->next = NULL;
    }
    else
    {
        *head = *tail = NULL;
    }

    free(temp);
}

Node *search(Node *head, int page)
{
    while (head)
    {
        if (head->page == page)
            return head;
        head = head->next;
    }
    return NULL;
}

void freeList(Node *head)
{
    while (head)
    {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

void printList(Node *head, int frame_count)
{
    int printed = 0;
    while (head)
    {
        printf("%d ", head->page);
        head = head->next;
        printed++;
    }
    while (printed < frame_count)
    {
        printf("- ");
        printed++;
    }
}

void printFrames(int frames[], int frame_count)
{
    for (int j = 0; j < frame_count; j++)
        (frames[j] == -1) ? printf("- ") : printf("%d ", frames[j]);
}

int isPageInFrames(int frames[], int frame_count, int page)
{
    for (int i = 0; i < frame_count; i++)
    {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

int getFrameIndex(int frames[], int frame_count, int page)
{
    for (int i = 0; i < frame_count; i++)
    {
        if (frames[i] == page)
            return i;
    }
    return -1;
}

void printPageReplacementSummary(const char *algorithm, int page_faults, int n)
{
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Page Faults (%-8s): %-7d║\n", algorithm, page_faults);
    printf("║ Page Hit Ratio: %-20.2f║\n", (float)(n - page_faults) / n);
    printf("║ Page Fault Ratio: %-18.2f║\n", (float)page_faults / n);
    printf("╚════════════════════════════════════════╝\n");
}

// ========================================
// FIFO PAGE REPLACEMENT
// ========================================

void runFIFO(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int front = 0;

    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;

    printf("\n========== FIFO PAGE REPLACEMENT ==========\n");
    printf("Strategy: Replace the oldest loaded page first\n");
    printf("Page | Frames\n");
    printf("-------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int hit = isPageInFrames(frames, frame_count, page);

        printf("%4d | ", page);

        if (!hit)
        {
            frames[front] = page;
            front = (front + 1) % frame_count;
            page_faults++;
        }

        printFrames(frames, frame_count);
        printf(hit ? " (Hit)\n" : " (Fault)\n");
    }

    printPageReplacementSummary("FIFO", page_faults, n);
}

// ========================================
// LRU PAGE REPLACEMENT
// ========================================

void runLRU(int pages[], int n, int frame_count)
{
    Node *head = NULL, *tail = NULL;
    int used = 0, faults = 0;

    printf("\n========== LRU PAGE REPLACEMENT ==========\n");
    printf("Strategy: Replace the least recently used page\n");
    printf("Page | Frames\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        Node *node = search(head, page);
        printf("%4d | ", page);

        if (node)
        {
            moveToHead(&head, &tail, node);
            printList(head, frame_count);
            printf(" (Hit)\n");
        }
        else
        {
            faults++;
            if (used < frame_count)
            {
                insertAtHead(&head, &tail, page);
                used++;
            }
            else
            {
                removeTail(&head, &tail);
                insertAtHead(&head, &tail, page);
            }
            printList(head, frame_count);
            printf(" (Fault)\n");
        }
    }

    printPageReplacementSummary("LRU", faults, n);
    freeList(head);
}

// ========================================
// OPTIMAL PAGE REPLACEMENT
// ========================================

int findOptimalVictim(int frames[], int frame_count, int pages[], int n, int current_index)
{
    int farthest_future_use = -1;
    int victim_index = -1;

    for (int i = 0; i < frame_count; i++)
    {
        int next_use = -1;

        for (int j = current_index + 1; j < n; j++)
        {
            if (frames[i] == pages[j])
            {
                next_use = j;
                break;
            }
        }

        if (next_use == -1)
            return i;

        if (next_use > farthest_future_use)
        {
            farthest_future_use = next_use;
            victim_index = i;
        }
    }

    return victim_index;
}

void runOptimal(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int used = 0;

    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;

    printf("\n========== OPTIMAL PAGE REPLACEMENT ==========\n");
    printf("Strategy: Replace the page used farthest in the future\n");
    printf("Page | Frames | Decision\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int hit = isPageInFrames(frames, frame_count, page);

        printf("%4d | ", page);

        if (hit)
        {
            printFrames(frames, frame_count);
            printf("| Hit\n");
            continue;
        }

        page_faults++;

        if (used < frame_count)
        {
            frames[used++] = page;
            printFrames(frames, frame_count);
            printf("| Fault - Empty frame used\n");
        }
        else
        {
            int victim_index = findOptimalVictim(frames, frame_count, pages, n, i);
            int replaced_page = frames[victim_index];
            frames[victim_index] = page;
            printFrames(frames, frame_count);
            printf("| Fault - Replaced %d\n", replaced_page);
        }
    }

    printPageReplacementSummary("Optimal", page_faults, n);
}

// ========================================
// LFU PAGE REPLACEMENT
// ========================================

int findLFUVictim(int frequencies[], int load_order[], int frame_count)
{
    int victim_index = 0;

    for (int i = 1; i < frame_count; i++)
    {
        if (frequencies[i] < frequencies[victim_index])
        {
            victim_index = i;
        }
        else if (frequencies[i] == frequencies[victim_index] && load_order[i] < load_order[victim_index])
        {
            victim_index = i;
        }
    }

    return victim_index;
}

void runLFU(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int frequencies[MAX_FRAMES];
    int load_order[MAX_FRAMES];
    int page_faults = 0;
    int used = 0;
    int time_counter = 0;

    for (int i = 0; i < frame_count; i++)
    {
        frames[i] = -1;
        frequencies[i] = 0;
        load_order[i] = 0;
    }

    printf("\n========== LFU PAGE REPLACEMENT ==========\n");
    printf("Strategy: Replace the least frequently used page; FIFO is used as tie-breaker\n");
    printf("Page | Frames | Frequencies | Decision\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int index = getFrameIndex(frames, frame_count, page);
        time_counter++;

        printf("%4d | ", page);

        if (index != -1)
        {
            frequencies[index]++;
            printFrames(frames, frame_count);
            printf("| ");
            for (int j = 0; j < frame_count; j++)
                (frames[j] == -1) ? printf("- ") : printf("%d ", frequencies[j]);
            printf("| Hit\n");
            continue;
        }

        page_faults++;

        if (used < frame_count)
        {
            frames[used] = page;
            frequencies[used] = 1;
            load_order[used] = time_counter;
            used++;

            printFrames(frames, frame_count);
            printf("| ");
            for (int j = 0; j < frame_count; j++)
                (frames[j] == -1) ? printf("- ") : printf("%d ", frequencies[j]);
            printf("| Fault - Empty frame used\n");
        }
        else
        {
            int victim_index = findLFUVictim(frequencies, load_order, frame_count);
            int replaced_page = frames[victim_index];

            frames[victim_index] = page;
            frequencies[victim_index] = 1;
            load_order[victim_index] = time_counter;

            printFrames(frames, frame_count);
            printf("| ");
            for (int j = 0; j < frame_count; j++)
                printf("%d ", frequencies[j]);
            printf("| Fault - Replaced %d\n", replaced_page);
        }
    }

    printPageReplacementSummary("LFU", page_faults, n);
}

#endif // PAGE_REPLACEMENT_H
