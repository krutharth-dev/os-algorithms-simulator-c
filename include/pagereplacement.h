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
    printf("Page | Frames\n");
    printf("-------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        int page = pages[i];
        int hit = 0;

        for (int j = 0; j < frame_count; j++)
            if (frames[j] == page)
                hit = 1;

        printf("%4d | ", page);

        if (!hit)
        {
            frames[front] = page;
            front = (front + 1) % frame_count;
            page_faults++;
        }

        for (int j = 0; j < frame_count; j++)
            (frames[j] == -1) ? printf("- ") : printf("%d ", frames[j]);

        printf(hit ? " (Hit)\n" : " (Fault)\n");
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Page Faults (FIFO): %-12d║\n", page_faults);
    printf("╚════════════════════════════════════════╝\n");
}

// ========================================
// LRU PAGE REPLACEMENT
// ========================================

void runLRU(int pages[], int n, int frame_count)
{
    Node *head = NULL, *tail = NULL;
    int used = 0, faults = 0;

    printf("\n========== LRU PAGE REPLACEMENT ==========\n");
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

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Page Faults (LRU): %-13d║\n", faults);
    printf("╚════════════════════════════════════════╝\n");
}

#endif // PAGE_REPLACEMENT_H