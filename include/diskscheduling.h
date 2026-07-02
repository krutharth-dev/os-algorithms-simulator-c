// diskscheduling.h
#ifndef DISK_SCHEDULING_H
#define DISK_SCHEDULING_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_REQUESTS 100
#define DISK_SIZE 200

// ========================================
// FCFS DISK SCHEDULING
// ========================================

void FCFS(int requests[], int n, int head)
{
    int seek_time = 0;
    int seek_sequence[MAX_REQUESTS + 1];
    seek_sequence[0] = head;

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
        seek_sequence[i + 1] = head;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Seek Time (FCFS): %-13d║\n", seek_time);
    printf("║ Average Seek Time: %-18.2f║\n", (float)seek_time / n);
    printf("╚════════════════════════════════════════╝\n");

    printf("\nSeek Sequence: ");
    for (int i = 0; i <= n; i++)
    {
        printf("%d", seek_sequence[i]);
        if (i < n)
            printf(" → ");
    }
    printf("\n");
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
    seek_sequence[0] = head;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      SSTF DISK SCHEDULING              ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Service closest request first\n");
    printf("Head starts at: %d\n\n", head);

    printf("Step | From | To  | Seek | Chosen (closest)\n");
    printf("------------------------------------------------\n");

    while (completed < n)
    {
        int min_distance = DISK_SIZE + 1;
        int index = -1;

        // Find closest request
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
        seek_sequence[completed + 1] = head;
        completed++;
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Seek Time (SSTF): %-13d║\n", seek_time);
    printf("║ Average Seek Time: %-18.2f║\n", (float)seek_time / n);
    printf("╚════════════════════════════════════════╝\n");

    printf("\nSeek Sequence: ");
    for (int i = 0; i <= n; i++)
    {
        printf("%d", seek_sequence[i]);
        if (i < n)
            printf(" → ");
    }
    printf("\n");
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

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║       SCAN DISK SCHEDULING             ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Move in one direction, service all requests, then reverse\n");
    printf("Head starts at: %d, Direction: %s\n\n", head, direction ? "Right (→)" : "Left (←)");

    seek_sequence[seq_index++] = head;

    // Separate requests into left and right of head
    for (int i = 0; i < n; i++)
    {
        if (requests[i] < head)
            left[lcount++] = requests[i];
        else
            right[rcount++] = requests[i];
    }

    // Sort left array (ascending)
    for (int i = 0; i < lcount - 1; i++)
        for (int j = i + 1; j < lcount; j++)
            if (left[i] > left[j])
            {
                int temp = left[i];
                left[i] = left[j];
                left[j] = temp;
            }

    // Sort right array (ascending)
    for (int i = 0; i < rcount - 1; i++)
        for (int j = i + 1; j < rcount; j++)
            if (right[i] > right[j])
            {
                int temp = right[i];
                right[i] = right[j];
                right[j] = temp;
            }

    printf("Step | From | To  | Seek | Phase\n");
    printf("------------------------------------------------\n");

    int step = 1;

    if (direction == 1)
    {
        // Move right first
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }

        // Go to end of disk
        if (head != DISK_SIZE - 1)
        {
            int dist = abs((DISK_SIZE - 1) - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | End of disk\n", step++, head, DISK_SIZE - 1, dist);
            head = DISK_SIZE - 1;
            seek_sequence[seq_index++] = head;
        }

        // Then move left
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
        // Move left first
        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }

        // Go to beginning of disk
        if (head != 0)
        {
            int dist = abs(head - 0);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Start of disk\n", step++, head, 0, dist);
            head = 0;
            seek_sequence[seq_index++] = head;
        }

        // Then move right
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Seek Time (SCAN): %-13d║\n", seek_time);
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
// C-SCAN DISK SCHEDULING
// ========================================

void CSCAN(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;
    int seek_sequence[MAX_REQUESTS + 4];
    int seq_index = 0;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      C-SCAN DISK SCHEDULING            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("Strategy: Move in one direction, jump to opposite end, continue\n");
    printf("Head starts at: %d, Direction: %s\n\n", head, direction ? "Right (→)" : "Left (←)");

    seek_sequence[seq_index++] = head;

    for (int i = 0; i < n; i++)
    {
        if (requests[i] < head)
            left[lcount++] = requests[i];
        else
            right[rcount++] = requests[i];
    }

    // Sort left ascending
    for (int i = 0; i < lcount - 1; i++)
        for (int j = i + 1; j < lcount; j++)
            if (left[i] > left[j])
            {
                int temp = left[i];
                left[i] = left[j];
                left[j] = temp;
            }

    // Sort right ascending
    for (int i = 0; i < rcount - 1; i++)
        for (int j = i + 1; j < rcount; j++)
            if (right[i] > right[j])
            {
                int temp = right[i];
                right[i] = right[j];
                right[j] = temp;
            }

    printf("Step | From | To  | Seek | Phase\n");
    printf("------------------------------------------------\n");

    int step = 1;

    if (direction == 1)
    {
        // Move right first
        for (int i = 0; i < rcount; i++)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Right\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }

        // Jump to end
        if (head != DISK_SIZE - 1)
        {
            int dist = abs((DISK_SIZE - 1) - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | End of disk\n", step++, head, DISK_SIZE - 1, dist);
            head = DISK_SIZE - 1;
            seek_sequence[seq_index++] = head;
        }

        // Jump to start (no seek cost shown separately)
        printf(" %-3d | %-4d | %-3d | %-4d | Jump to start\n", step++, head, 0, 0);
        head = 0;
        seek_sequence[seq_index++] = head;

        // Continue from start
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
        // Move left first
        for (int i = lcount - 1; i >= 0; i--)
        {
            int dist = abs(left[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Moving Left\n", step++, head, left[i], dist);
            head = left[i];
            seek_sequence[seq_index++] = head;
        }

        // Jump to beginning
        if (head != 0)
        {
            int dist = abs(head - 0);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Start of disk\n", step++, head, 0, dist);
            head = 0;
            seek_sequence[seq_index++] = head;
        }

        // Jump to end (no seek cost shown separately)
        printf(" %-3d | %-4d | %-3d | %-4d | Jump to end\n", step++, head, DISK_SIZE - 1, 0);
        head = DISK_SIZE - 1;
        seek_sequence[seq_index++] = head;

        // Continue from end
        for (int i = rcount - 1; i >= 0; i--)
        {
            int dist = abs(right[i] - head);
            seek_time += dist;
            printf(" %-3d | %-4d | %-3d | %-4d | Continuing Left\n", step++, head, right[i], dist);
            head = right[i];
            seek_sequence[seq_index++] = head;
        }
    }

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║ Total Seek Time (C-SCAN): %-11d║\n", seek_time);
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

#endif // DISK_SCHEDULING_H/n