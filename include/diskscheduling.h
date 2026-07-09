// diskscheduling.h
#ifndef DISK_SCHEDULING_H
#define DISK_SCHEDULING_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_REQUESTS 100
#define DISK_SIZE 200

void sortAscending(int arr[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            if (arr[i] > arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

void splitRequests(int requests[], int n, int head, int left[], int *lcount, int right[], int *rcount)
{
    *lcount = 0;
    *rcount = 0;

    for (int i = 0; i < n; i++)
    {
        if (requests[i] < head)
            left[(*lcount)++] = requests[i];
        else
            right[(*rcount)++] = requests[i];
    }

    sortAscending(left, *lcount);
    sortAscending(right, *rcount);
}

void printSeekSummary(const char *algorithm_name, int seek_time, int n, int seek_sequence[], int seq_index)
{
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Seek Time (%-7s): %-11d║\n", algorithm_name, seek_time);
    printf("║ Average Seek Time: %-18.2f║\n", (float)seek_time / n);
    printf("╚════════════════════════════════════════╝\n");

    printf("\nSeek Sequence: ");
    for (int i = 0; i < seq_index; i++)
    {
        printf("%d", seek_sequence[i]);
        if (i < seq_index - 1)
            printf(" → ");
    }
    printf("\n");
}

// ========================================
// FCFS DISK SCHEDULING
// ========================================

void FCFS(int requests[], int n, int head)
{
    int seek_time = 0;
    int seek_sequence[MAX_REQUESTS + 1];
    int seq_index = 0;
    seek_sequence[seq_index++] = head;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      FCFS DISK SCHEDULING              ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Service requests in order of arrival\n");
    printf("Head starts at: %d\n\n", head);

    printf("Step | From | To  | Seek\n");
    printf("----------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        int distance = abs(requests[i] - head);
        seek_time += distance;
        printf(" %-3d | %-4d | %-3d | %d\n", i + 1, head, requests[i], distance);
        head = requests[i];
        seek_sequence[seq_index++] = head;
    }

    printSeekSummary("FCFS", seek_time, n, seek_sequence, seq_index);
}

// ========================================
// SSTF DISK SCHEDULING
// ========================================

void SSTF(int requests[], int n, int head)
{
    bool visited[MAX_REQUESTS] = {false};
    int seek_time = 0;
    int completed = 0;
    int seek_sequence[MAX_REQUESTS + 1];
    int seq_index = 0;
    seek_sequence[seq_index++] = head;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      SSTF DISK SCHEDULING              ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Service closest request first\n");
    printf("Head starts at: %d\n\n", head);

    printf("Step | From | To  | Seek | Chosen\n");
    printf("------------------------------------------------\n");

    while (completed < n)
    {
        int min_distance = DISK_SIZE + 1;
        int index = -1;

        for (int i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                int dist = abs(requests[i] - head);
                if (dist < min_distance)
                {
                    min_distance = dist;
                    index = i;
                }
            }
        }

        visited[index] = true;
        seek_time += min_distance;
        printf(" %-3d | %-4d | %-3d | %-4d | Request #%d\n",
               completed + 1, head, requests[index], min_distance, index + 1);
        head = requests[index];
        seek_sequence[seq_index++] = head;
        completed++;
    }

    printSeekSummary("SSTF", seek_time, n, seek_sequence, seq_index);
}

// ========================================
// SCAN DISK SCHEDULING
// ========================================

void SCAN(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;
    int seek_sequence[MAX_REQUESTS + 3];
    int seq_index = 0;
    int step = 1;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║       SCAN DISK SCHEDULING             ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Move to disk end, then reverse direction\n");
    printf("Head starts at: %d, Direction: %s\n\n", head, direction ? "Right (→)" : "Left (←)");

    seek_sequence[seq_index++] = head;
    splitRequests(requests, n, head, left, &lcount, right, &rcount);

    printf("Step | From | To  | Seek | Phase\n");
    printf("------------------------------------------------\n");

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }

        if (head != DISK_SIZE - 1)
        {
            int dist = abs((DISK_SIZE - 1) - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | End of disk\n", step++, head, DISK_SIZE - 1, dist);
            head = DISK_SIZE - 1;
            seek_sequence[seq_index++] = head;
        }

        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }

        if (head != 0)
        {
            int dist = abs(head - 0);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Start of disk\n", step++, head, 0, dist);
            head = 0;
            seek_sequence[seq_index++] = head;
        }

        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }
    }

    printSeekSummary("SCAN", seek_time, n, seek_sequence, seq_index);
}

// ========================================
// C-SCAN DISK SCHEDULING
// ========================================

void CSCAN(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;
    int seek_sequence[MAX_REQUESTS + 4];
    int seq_index = 0;
    int step = 1;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      C-SCAN DISK SCHEDULING            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Move to disk end, jump to opposite end, continue\n");
    printf("Head starts at: %d, Direction: %s\n\n", head, direction ? "Right (→)" : "Left (←)");

    seek_sequence[seq_index++] = head;
    splitRequests(requests, n, head, left, &lcount, right, &rcount);

    printf("Step | From | To  | Seek | Phase\n");
    printf("------------------------------------------------\n");

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }

        if (head != DISK_SIZE - 1)
        {
            int dist = abs((DISK_SIZE - 1) - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | End of disk\n", step++, head, DISK_SIZE - 1, dist);
            head = DISK_SIZE - 1;
            seek_sequence[seq_index++] = head;
        }

        printf(" %-3d | %-4d | %-3d | %-4d | Jump to start\n", step++, head, 0, 0);
        head = 0;
        seek_sequence[seq_index++] = head;

        for (int i = 0; i < lcount; i++)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Continuing Right\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }

        if (head != 0)
        {
            int dist = abs(head - 0);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Start of disk\n", step++, head, 0, dist);
            head = 0;
            seek_sequence[seq_index++] = head;
        }

        printf(" %-3d | %-4d | %-3d | %-4d | Jump to end\n", step++, head, DISK_SIZE - 1, 0);
        head = DISK_SIZE - 1;
        seek_sequence[seq_index++] = head;

        for (int i = rcount - 1; i >= 0; i--)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Continuing Left\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }
    }

    printSeekSummary("C-SCAN", seek_time, n, seek_sequence, seq_index);
}

// ========================================
// LOOK DISK SCHEDULING
// ========================================

void LOOK(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;
    int seek_sequence[MAX_REQUESTS + 1];
    int seq_index = 0;
    int step = 1;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║       LOOK DISK SCHEDULING             ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Move only as far as the last request, then reverse\n");
    printf("Head starts at: %d, Direction: %s\n\n", head, direction ? "Right (→)" : "Left (←)");

    seek_sequence[seq_index++] = head;
    splitRequests(requests, n, head, left, &lcount, right, &rcount);

    printf("Step | From | To  | Seek | Phase\n");
    printf("------------------------------------------------\n");

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }

        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Reversing Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }

        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Reversing Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }
    }

    printSeekSummary("LOOK", seek_time, n, seek_sequence, seq_index);
}

// ========================================
// C-LOOK DISK SCHEDULING
// ========================================

void CLOOK(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;
    int seek_sequence[MAX_REQUESTS + 1];
    int seq_index = 0;
    int step = 1;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      C-LOOK DISK SCHEDULING            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Move to last request, jump to first request, continue\n");
    printf("Head starts at: %d, Direction: %s\n\n", head, direction ? "Right (→)" : "Left (←)");

    seek_sequence[seq_index++] = head;
    splitRequests(requests, n, head, left, &lcount, right, &rcount);

    printf("Step | From | To  | Seek | Phase\n");
    printf("------------------------------------------------\n");

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }

        if (lcount > 0)
        {
            int dist = abs(head - left[0]);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Circular jump\n", step++, head, left[0], dist);
            head = left[0];
            seek_sequence[seq_index++] = head;

            for (int i = 1; i < lcount; i++)
            {
                dist = abs(left[i] - head);
                seek_time += dist;
                printf(" %-3d | %-4d | %-3d | %-4d | Continuing Right\n", step++, head, left[i], dist);
                head = left[i];
                seek_sequence[seq_index++] = head;
            }
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }

        if (rcount > 0)
        {
            int dist = abs(right[rcount - 1] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Circular jump\n", step++, head, right[rcount - 1], dist);
            head = right[rcount - 1];
            seek_sequence[seq_index++] = head;

            for (int i = rcount - 2; i >= 0; i--)
            {
                dist = abs(right[i] - head);
                seek_time += dist;
                printf(" %-3d | %-4d | %-3d | %-4d | Continuing Left\n", step++, head, right[i], dist);
                head = right[i];
                seek_sequence[seq_index++] = head;
            }
        }
    }

    printSeekSummary("C-LOOK", seek_time, n, seek_sequence, seq_index);
}

#endif // DISK_SCHEDULING_H
