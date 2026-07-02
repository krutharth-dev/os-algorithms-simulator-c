// ipc.h
#ifndef IPC_H
#define IPC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 5
#define SHM_SIZE 1024

// ========================================
// PIPE IPC - PARENT-CHILD COMMUNICATION
// ========================================

void runPipeIPC()
{
    int pipefd[2];
    pid_t pid;
    char write_msg[100];
    char read_msg[100];

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║      PIPE IPC DEMONSTRATION            ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\nConcept: Unidirectional communication between parent and child\n");
    printf("Mechanism: pipe() system call creates two file descriptors\n");
    printf("  - pipefd[0]: Read end\n");
    printf("  - pipefd[1]: Write end\n");

    printf("\n────────────────────────────────────────\n");
    printf("Enter message to send from parent to child: ");
    getchar();
    fgets(write_msg, sizeof(write_msg), stdin);
    write_msg[strcspn(write_msg, "\n")] = 0;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }

    printf("\n✓ Pipe created successfully\n");
    printf("  Read FD: %d, Write FD: %d\n", pipefd[0], pipefd[1]);

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }
    else if (pid > 0)
    {
        close(pipefd[0]);

        printf("\n[PARENT PROCESS - PID: %d]\n", getpid());
        printf("  ├─ Closing read end of pipe\n");
        printf("  ├─ Writing message: '%s'\n", write_msg);
        printf("  ├─ Message length: %lu bytes\n", strlen(write_msg) + 1);

        write(pipefd[1], write_msg, strlen(write_msg) + 1);

        printf("  ├─ Message written to pipe\n");
        printf("  └─ Closing write end\n");

        close(pipefd[1]);
        wait(NULL);

        printf("\n[PARENT] ✓ Child process completed. Communication successful!\n");
    }
    else
    {
        close(pipefd[1]);

        printf("\n[CHILD PROCESS - PID: %d, Parent PID: %d]\n", getpid(), getppid());
        printf("  ├─ Closing write end of pipe\n");
        printf("  ├─ Reading from pipe...\n");

        read(pipefd[0], read_msg, sizeof(read_msg));

        printf("  ├─ Message received: '%s'\n", read_msg);
        printf("  ├─ Message length: %lu bytes\n", strlen(read_msg));
        printf("  └─ Closing read end\n");

        close(pipefd[0]);

        printf("\n[CHILD] ✓ Message received successfully!\n");
        exit(EXIT_SUCCESS);
    }

    printf("\n════════════════════════════════════════\n");
}

// ========================================
// PRODUCER-CONSUMER WITH SEMAPHORES
// ========================================

int buffer[BUFFER_SIZE];
int in_pc = 0;
int out_pc = 0;

sem_t *full_sem;
sem_t *empty_sem;

pthread_mutex_t mutex_pc;

int total_items = 10;
int items_produced = 0;
int items_consumed = 0;

void *producer(void *arg)
{
    (void)arg;

    int item = 1;

    while (items_produced < total_items)
    {
        sem_wait(empty_sem);

        pthread_mutex_lock(&mutex_pc);

        buffer[in_pc] = item;

        printf("✓ Producer produced: Item %d → Buffer[%d]\n", item, in_pc);

        in_pc = (in_pc + 1) % BUFFER_SIZE;
        items_produced++;

        pthread_mutex_unlock(&mutex_pc);

        sem_post(full_sem);

        item++;
        sleep(1);
    }

    printf("\n🏁 Producer finished! Total items produced: %d\n", total_items);
    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;

    while (items_consumed < total_items)
    {
        sem_wait(full_sem);

        pthread_mutex_lock(&mutex_pc);

        int item = buffer[out_pc];

        printf("✓ Consumer consumed: Item %d ← Buffer[%d]\n", item, out_pc);

        out_pc = (out_pc + 1) % BUFFER_SIZE;
        items_consumed++;

        pthread_mutex_unlock(&mutex_pc);

        sem_post(empty_sem);

        sleep(2);
    }

    printf("\n🏁 Consumer finished! Total items consumed: %d\n", total_items);
    return NULL;
}

void runProducerConsumer()
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║   PRODUCER-CONSUMER WITH SEMAPHORES    ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("\nConcept: Classic synchronization problem using bounded buffer\n");
    printf("Synchronization Tools:\n");
    printf("  • Semaphore 'full': Counts full slots\n");
    printf("  • Semaphore 'empty': Counts empty slots\n");
    printf("  • Mutex: Ensures mutual exclusion in critical section\n");

    printf("\nConfiguration:\n");
    printf("  Buffer Size: %d slots\n", BUFFER_SIZE);
    printf("  Total Items to Produce/Consume: %d\n", total_items);
    printf("  Producer Speed: 1 item/second\n");
    printf("  Consumer Speed: 1 item/2 seconds\n");

    items_produced = 0;
    items_consumed = 0;
    in_pc = 0;
    out_pc = 0;

    printf("\n────────────────────────────────────────\n");
    printf("Initializing synchronization primitives...\n");

    sem_unlink("/full_sem");
    sem_unlink("/empty_sem");

    full_sem = sem_open("/full_sem", O_CREAT, 0644, 0);
    empty_sem = sem_open("/empty_sem", O_CREAT, 0644, BUFFER_SIZE);

    if (full_sem == SEM_FAILED || empty_sem == SEM_FAILED)
    {
        perror("sem_open");
        return;
    }

    printf("✓ Semaphores created\n");

    if (pthread_mutex_init(&mutex_pc, NULL) != 0)
    {
        perror("pthread_mutex_init");
        sem_close(full_sem);
        sem_close(empty_sem);
        sem_unlink("/full_sem");
        sem_unlink("/empty_sem");
        return;
    }

    printf("✓ Mutex initialized\n");

    printf("\n────────────────────────────────────────\n");
    printf("Starting Producer and Consumer threads...\n\n");

    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0)
    {
        perror("pthread_create producer");
        return;
    }

    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0)
    {
        perror("pthread_create consumer");
        return;
    }

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("\n════════════════════════════════════════\n");
    printf("        EXECUTION SUMMARY\n");
    printf("════════════════════════════════════════\n");
    printf("Total Produced: %d items\n", items_produced);
    printf("Total Consumed: %d items\n", items_consumed);
    printf("Final Buffer State: %s\n",
           (items_produced == items_consumed) ? "Empty (Synchronized!)" : "Has items");
    printf("════════════════════════════════════════\n\n");

    sem_close(full_sem);
    sem_close(empty_sem);
    sem_unlink("/full_sem");
    sem_unlink("/empty_sem");
    pthread_mutex_destroy(&mutex_pc);
}

// ========================================
// SHARED MEMORY IPC - WRITER
// ========================================

void runSharedMemoryWriter()
{
    key_t key;
    int shmid;
    char *data;
    char input[SHM_SIZE];

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     SHARED MEMORY - WRITER             ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("\nConcept: Fastest IPC mechanism - processes share memory region\n");
    printf("System Calls Used:\n");
    printf("  1. ftok()   - Generate unique key\n");
    printf("  2. shmget() - Create/access shared memory segment\n");
    printf("  3. shmat()  - Attach segment to process address space\n");
    printf("  4. shmdt()  - Detach segment from process\n");

    FILE *fp = fopen("shmfile", "w");
    if (fp)
    {
        fclose(fp);
        printf("\n✓ Key file 'shmfile' created\n");
    }

    printf("\n────────────────────────────────────────\n");
    printf("Step 1: Generating unique key...\n");

    key = ftok("shmfile", 65);
    if (key == -1)
    {
        perror("ftok");
        return;
    }

    printf("✓ Key generated: 0x%x\n", key);

    printf("\nStep 2: Creating shared memory segment...\n");
    printf("  Size: %d bytes\n", SHM_SIZE);
    printf("  Permissions: 0666\n");

    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        perror("shmget");
        return;
    }

    printf("✓ Shared memory created\n");
    printf("  Segment ID: %d\n", shmid);

    printf("\nStep 3: Attaching shared memory to process...\n");

    data = (char *)shmat(shmid, NULL, 0);
    if (data == (char *)-1)
    {
        perror("shmat");
        return;
    }

    printf("✓ Shared memory attached at address: %p\n", (void *)data);

    printf("\n────────────────────────────────────────\n");
    printf("Enter data to write to shared memory: ");
    getchar();
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    strncpy(data, input, SHM_SIZE - 1);
    data[SHM_SIZE - 1] = '\0';

    printf("\n✓ Data written to shared memory\n");
    printf("  Content: '%s'\n", data);
    printf("  Length: %lu bytes\n", strlen(data));

    printf("\nStep 4: Detaching shared memory...\n");

    if (shmdt(data) == -1)
    {
        perror("shmdt");
        return;
    }

    printf("✓ Shared memory detached\n");

    printf("\n════════════════════════════════════════\n");
    printf("Writer Summary:\n");
    printf("  Segment ID: %d\n", shmid);
    printf("  Key: 0x%x\n", key);
    printf("  Status: Data written and ready to read\n");
    printf("\n💡 Now run the Reader option to read this data!\n");
    printf("════════════════════════════════════════\n\n");
}

// ========================================
// SHARED MEMORY IPC - READER
// ========================================

void runSharedMemoryReader()
{
    key_t key;
    int shmid;
    char *data;

    printf("\n╔════════════════════════════════════════╗\n");
    printf("║     SHARED MEMORY - READER             ║\n");
    printf("╚════════════════════════════════════════╝\n");

    printf("\nConcept: Reading from shared memory created by Writer\n");
    printf("Note: Writer must run first to create the segment\n");

    printf("\n────────────────────────────────────────\n");
    printf("Step 1: Generating key to locate segment...\n");

    key = ftok("shmfile", 65);
    if (key == -1)
    {
        perror("ftok");
        printf("\n⚠️  Run Writer before Reader\n");
        return;
    }

    printf("✓ Key generated: 0x%x\n", key);

    printf("\nStep 2: Locating shared memory segment...\n");

    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1)
    {
        perror("shmget");
        printf("\n⚠️  Shared memory segment does not exist\n");
        printf("    Please run Writer first!\n");
        return;
    }

    printf("✓ Shared memory segment found\n");
    printf("  Segment ID: %d\n", shmid);

    printf("\nStep 3: Attaching to shared memory...\n");

    data = (char *)shmat(shmid, NULL, 0);
    if (data == (char *)-1)
    {
        perror("shmat");
        return;
    }

    printf("✓ Shared memory attached at address: %p\n", (void *)data);

    printf("\n────────────────────────────────────────\n");
    printf("Reading data from shared memory...\n\n");

    printf("✓ Data read successfully!\n");
    printf("  Content: '%s'\n", data);
    printf("  Length: %lu bytes\n", strlen(data));

    printf("\nStep 4: Detaching from shared memory...\n");

    if (shmdt(data) == -1)
    {
        perror("shmdt");
        return;
    }

    printf("✓ Shared memory detached\n");

    printf("\n════════════════════════════════════════\n");
    printf("Reader Summary:\n");
    printf("  Segment ID: %d\n", shmid);
    printf("  Key: 0x%x\n", key);
    printf("  Status: Data read successfully\n");
    printf("\n💡 Segment still exists for other readers\n");
    printf("   Use this command to remove it manually:\n");
    printf("   ipcrm -m %d\n", shmid);
    printf("════════════════════════════════════════\n\n");
}

#endif // IPC_H