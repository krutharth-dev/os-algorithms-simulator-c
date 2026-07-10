#ifndef ALGORITHM_REPORTS_H
#define ALGORITHM_REPORTS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ==========================================================
// PAGE REPLACEMENT REPORT HELPERS
// ==========================================================

int report_page_in_frames(int frames[], int frame_count, int page)
{
    for (int i = 0; i < frame_count; i++)
    {
        if (frames[i] == page)
            return 1;
    }
    return 0;
}

int report_frame_index(int frames[], int frame_count, int page)
{
    for (int i = 0; i < frame_count; i++)
    {
        if (frames[i] == page)
            return i;
    }
    return -1;
}

int report_find_optimal_victim(int frames[], int frame_count, int pages[], int n, int current_index)
{
    int farthest_future_use = -1;
    int victim_index = 0;

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

int calculate_fifo_faults(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int front = 0;
    int faults = 0;

    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;

    for (int i = 0; i < n; i++)
    {
        if (!report_page_in_frames(frames, frame_count, pages[i]))
        {
            frames[front] = pages[i];
            front = (front + 1) % frame_count;
            faults++;
        }
    }

    return faults;
}

int calculate_lru_faults(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];
    int used = 0;
    int faults = 0;

    for (int i = 0; i < frame_count; i++)
    {
        frames[i] = -1;
        last_used[i] = -1;
    }

    for (int i = 0; i < n; i++)
    {
        int index = report_frame_index(frames, frame_count, pages[i]);

        if (index != -1)
        {
            last_used[index] = i;
            continue;
        }

        faults++;

        if (used < frame_count)
        {
            frames[used] = pages[i];
            last_used[used] = i;
            used++;
        }
        else
        {
            int victim_index = 0;

            for (int j = 1; j < frame_count; j++)
            {
                if (last_used[j] < last_used[victim_index])
                    victim_index = j;
            }

            frames[victim_index] = pages[i];
            last_used[victim_index] = i;
        }
    }

    return faults;
}

int calculate_optimal_faults(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int used = 0;
    int faults = 0;

    for (int i = 0; i < frame_count; i++)
        frames[i] = -1;

    for (int i = 0; i < n; i++)
    {
        if (report_page_in_frames(frames, frame_count, pages[i]))
            continue;

        faults++;

        if (used < frame_count)
        {
            frames[used++] = pages[i];
        }
        else
        {
            int victim_index = report_find_optimal_victim(frames, frame_count, pages, n, i);
            frames[victim_index] = pages[i];
        }
    }

    return faults;
}

int report_find_lfu_victim(int frequencies[], int load_order[], int frame_count)
{
    int victim_index = 0;

    for (int i = 1; i < frame_count; i++)
    {
        if (frequencies[i] < frequencies[victim_index])
            victim_index = i;
        else if (frequencies[i] == frequencies[victim_index] && load_order[i] < load_order[victim_index])
            victim_index = i;
    }

    return victim_index;
}

int calculate_lfu_faults(int pages[], int n, int frame_count)
{
    int frames[MAX_FRAMES];
    int frequencies[MAX_FRAMES];
    int load_order[MAX_FRAMES];
    int used = 0;
    int faults = 0;
    int time_counter = 0;

    for (int i = 0; i < frame_count; i++)
    {
        frames[i] = -1;
        frequencies[i] = 0;
        load_order[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        time_counter++;
        int index = report_frame_index(frames, frame_count, pages[i]);

        if (index != -1)
        {
            frequencies[index]++;
            continue;
        }

        faults++;

        if (used < frame_count)
        {
            frames[used] = pages[i];
            frequencies[used] = 1;
            load_order[used] = time_counter;
            used++;
        }
        else
        {
            int victim_index = report_find_lfu_victim(frequencies, load_order, frame_count);
            frames[victim_index] = pages[i];
            frequencies[victim_index] = 1;
            load_order[victim_index] = time_counter;
        }
    }

    return faults;
}

void print_page_replacement_comparison(int pages[], int n, int frame_count)
{
    int fifo_faults = calculate_fifo_faults(pages, n, frame_count);
    int lru_faults = calculate_lru_faults(pages, n, frame_count);
    int optimal_faults = calculate_optimal_faults(pages, n, frame_count);
    int lfu_faults = calculate_lfu_faults(pages, n, frame_count);

    printf("\n================ PAGE REPLACEMENT COMPARISON ================\n");
    printf("%-12s %-12s %-12s %-12s\n", "Algorithm", "Faults", "Hit Ratio", "Fault Ratio");
    printf("--------------------------------------------------------------\n");
    printf("%-12s %-12d %-12.2f %-12.2f\n", "FIFO", fifo_faults, (float)(n - fifo_faults) / n, (float)fifo_faults / n);
    printf("%-12s %-12d %-12.2f %-12.2f\n", "LRU", lru_faults, (float)(n - lru_faults) / n, (float)lru_faults / n);
    printf("%-12s %-12d %-12.2f %-12.2f\n", "Optimal", optimal_faults, (float)(n - optimal_faults) / n, (float)optimal_faults / n);
    printf("%-12s %-12d %-12.2f %-12.2f\n", "LFU", lfu_faults, (float)(n - lfu_faults) / n, (float)lfu_faults / n);
    printf("==============================================================\n");
}

// ==========================================================
// DISK SCHEDULING REPORT HELPERS
// ==========================================================

void report_sort_ascending(int arr[], int count)
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

void report_split_requests(int requests[], int n, int head, int left[], int *lcount, int right[], int *rcount)
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

    report_sort_ascending(left, *lcount);
    report_sort_ascending(right, *rcount);
}

int calculate_fcfs_seek(int requests[], int n, int head)
{
    int seek_time = 0;

    for (int i = 0; i < n; i++)
    {
        seek_time += abs(requests[i] - head);
        head = requests[i];
    }

    return seek_time;
}

int calculate_sstf_seek(int requests[], int n, int head)
{
    bool visited[MAX_REQUESTS] = {false};
    int seek_time = 0;
    int completed = 0;

    while (completed < n)
    {
        int min_distance = DISK_SIZE + 1;
        int index = -1;

        for (int i = 0; i < n; i++)
        {
            if (!visited[i])
            {
                int distance = abs(requests[i] - head);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    index = i;
                }
            }
        }

        visited[index] = true;
        seek_time += min_distance;
        head = requests[index];
        completed++;
    }

    return seek_time;
}

int calculate_scan_seek(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;

    report_split_requests(requests, n, head, left, &lcount, right, &rcount);

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }

        if (head != DISK_SIZE - 1)
        {
            seek_time += abs((DISK_SIZE - 1) - head);
            head = DISK_SIZE - 1;
        }

        for (int i = lcount - 1; i >= 0; i--)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }

        if (head != 0)
        {
            seek_time += abs(head - 0);
            head = 0;
        }

        for (int i = 0; i < rcount; i++)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }
    }

    return seek_time;
}

int calculate_cscan_seek(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;

    report_split_requests(requests, n, head, left, &lcount, right, &rcount);

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }

        if (head != DISK_SIZE - 1)
        {
            seek_time += abs((DISK_SIZE - 1) - head);
            head = DISK_SIZE - 1;
        }

        head = 0;

        for (int i = 0; i < lcount; i++)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }

        if (head != 0)
        {
            seek_time += abs(head - 0);
            head = 0;
        }

        head = DISK_SIZE - 1;

        for (int i = rcount - 1; i >= 0; i--)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }
    }

    return seek_time;
}

int calculate_look_seek(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;

    report_split_requests(requests, n, head, left, &lcount, right, &rcount);

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }

        for (int i = lcount - 1; i >= 0; i--)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }

        for (int i = 0; i < rcount; i++)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }
    }

    return seek_time;
}

int calculate_clook_seek(int requests[], int n, int head, int direction)
{
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int lcount = 0, rcount = 0;

    report_split_requests(requests, n, head, left, &lcount, right, &rcount);

    if (direction == 1)
    {
        for (int i = 0; i < rcount; i++)
        {
            seek_time += abs(right[i] - head);
            head = right[i];
        }

        if (lcount > 0)
        {
            seek_time += abs(head - left[0]);
            head = left[0];

            for (int i = 1; i < lcount; i++)
            {
                seek_time += abs(left[i] - head);
                head = left[i];
            }
        }
    }
    else
    {
        for (int i = lcount - 1; i >= 0; i--)
        {
            seek_time += abs(left[i] - head);
            head = left[i];
        }

        if (rcount > 0)
        {
            seek_time += abs(right[rcount - 1] - head);
            head = right[rcount - 1];

            for (int i = rcount - 2; i >= 0; i--)
            {
                seek_time += abs(right[i] - head);
                head = right[i];
            }
        }
    }

    return seek_time;
}

void print_disk_scheduling_comparison(int requests[], int n, int head, int direction)
{
    int fcfs_seek = calculate_fcfs_seek(requests, n, head);
    int sstf_seek = calculate_sstf_seek(requests, n, head);
    int scan_seek = calculate_scan_seek(requests, n, head, direction);
    int cscan_seek = calculate_cscan_seek(requests, n, head, direction);
    int look_seek = calculate_look_seek(requests, n, head, direction);
    int clook_seek = calculate_clook_seek(requests, n, head, direction);

    printf("\n================== DISK SCHEDULING COMPARISON ==================\n");
    printf("Direction used for directional algorithms: %s\n", direction ? "Right" : "Left");
    printf("%-12s %-18s %-18s\n", "Algorithm", "Total Seek Time", "Average Seek Time");
    printf("-----------------------------------------------------------------\n");
    printf("%-12s %-18d %-18.2f\n", "FCFS", fcfs_seek, (float)fcfs_seek / n);
    printf("%-12s %-18d %-18.2f\n", "SSTF", sstf_seek, (float)sstf_seek / n);
    printf("%-12s %-18d %-18.2f\n", "SCAN", scan_seek, (float)scan_seek / n);
    printf("%-12s %-18d %-18.2f\n", "C-SCAN", cscan_seek, (float)cscan_seek / n);
    printf("%-12s %-18d %-18.2f\n", "LOOK", look_seek, (float)look_seek / n);
    printf("%-12s %-18d %-18.2f\n", "C-LOOK", clook_seek, (float)clook_seek / n);
    printf("=================================================================\n");
}

#endif // ALGORITHM_REPORTS_H
