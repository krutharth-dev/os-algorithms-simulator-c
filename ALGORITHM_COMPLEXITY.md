# Algorithm Complexity Reference

This document summarises the main algorithms implemented in the Operating System Algorithms Simulator and the expected time/space complexity for each educational simulation.

---

## Page Replacement Algorithms

| Algorithm | Time Complexity | Space Complexity | Notes |
|---|---:|---:|---|
| FIFO | O(n × f) | O(f) | For each page, frames are checked for a hit. Replacement uses a circular pointer. |
| LRU | O(n × f) | O(f) | Current implementation searches the linked list for each page reference. |
| Optimal | O(n × f × n) | O(f) | For each fault, future references are scanned to find the page used farthest in the future. |
| LFU | O(n × f) | O(f) | Frequency counters are maintained for pages currently in frames. FIFO load order is used as a tie-breaker. |

Where:

```text
n = number of page references
f = number of frames
```

---

## Memory Allocation Algorithms

| Algorithm | Time Complexity | Space Complexity | Notes |
|---|---:|---:|---|
| First Fit | O(p × b) | O(b) | Each process scans blocks until the first suitable block is found. |
| Best Fit | O(p × b) | O(b) | Each process scans all blocks to find the smallest fitting block. |
| Worst Fit | O(p × b) | O(b) | Each process scans all blocks to find the largest available block. |

Where:

```text
p = number of processes
b = number of memory blocks
```

---

## Disk Scheduling Algorithms

| Algorithm | Time Complexity | Space Complexity | Notes |
|---|---:|---:|---|
| FCFS | O(n) | O(n) | Services requests in arrival order. |
| SSTF | O(n²) | O(n) | Repeatedly searches for the closest unvisited request. |
| SCAN | O(n²) | O(n) | Current sorting uses simple comparison sorting before scanning. |
| C-SCAN | O(n²) | O(n) | Similar to SCAN but jumps to the opposite end after reaching one end. |
| LOOK | O(n²) | O(n) | Similar to SCAN but only moves as far as the last request. |
| C-LOOK | O(n²) | O(n) | Similar to C-SCAN but jumps between actual request limits instead of disk ends. |

Where:

```text
n = number of disk requests
```

Note: Sorting can be improved to O(n log n) if a more efficient sorting algorithm is used.

---

## IPC Demonstrations

| Module | Main Concept | Complexity Focus |
|---|---|---|
| Pipe Communication | Parent-child communication through pipes | Process communication rather than algorithmic complexity |
| Producer-Consumer | Semaphores and synchronisation | Thread/process coordination |
| Shared Memory Writer/Reader | Shared memory segment usage | IPC mechanism demonstration |

---

## Portfolio Explanation

This project is useful because it does not only implement the algorithms; it also allows comparison between multiple strategies. The simulator can be discussed in terms of:

- Correctness of implementation
- Algorithmic complexity
- Memory usage
- Practical trade-offs
- Educational visualisation through terminal output

---

## Suggested Future Improvement

A future version can add a final comparison table after each `Run All` option, allowing users to directly compare total page faults or total seek time across algorithms.
