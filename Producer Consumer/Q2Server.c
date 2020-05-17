#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h> // for semaphore operations sem_init,sem_wait,sem_post
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <math.h>

extern int errno;
#define true 1
#define SIZE 10  /* size of the shared buffer*/
#define VARSIZE 1 /* size of shared variable=1byte*/
#define INPUTSIZE 20
#define SHMPERM 0666 /* shared memory permissions */
int segid;           /* id for shared memory bufer */
int empty_id;
int full_id;
int mutex_id;
int *buff;
sem_t *empty;
sem_t *full;
sem_t *mutex;
int main()
{
    pid_t temp_pid;
    segid = shmget(315, SIZE * sizeof(int), IPC_CREAT | IPC_EXCL | SHMPERM);
    empty_id = shmget(316, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
    full_id = shmget(317, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
    mutex_id = shmget(318, sizeof(sem_t), IPC_CREAT | IPC_EXCL | SHMPERM);
    buff = shmat(segid, (char *)0, 0);
    empty = shmat(empty_id, (char *)0, 0);
    full = shmat(full_id, (char *)0, 0);
    mutex = shmat(mutex_id, (char *)0, 0);
    // Initializing Semaphores Empty , Full & Mutex
    sem_init(empty, 1, SIZE);
    sem_init(full, 1, 0);
    sem_init(mutex, 1, 1);
    printf("\nProducer / Server Process Started \n");

    sleep(2);

    unsigned int i = 0,p = 0;
    while (true)
    {
        if (i >= SIZE)
        {
            printf("\n Producer %d exited \n", getpid());
            wait(NULL);
            exit(1);
        }

        printf("\nProducer %d trying to aquire Semaphore Empty \n", getpid());
        sem_wait(empty);
        printf("\nProducer %d successfully aquired Semaphore Empty \n", getpid());
        
        printf("\nProducer %d trying to aquire Semaphore Mutex \n", getpid());
        sem_wait(mutex);
        printf("\nProducer %d successfully aquired Semaphore Mutex \n", getpid());
        
        buff[p] = (random() % 10 + 1);
        printf("\nProducer %d Produced Item [ %d ] \n", getpid(), buff[p]);
        i++;
        p++;
        printf("\nItems in Buffer %d \n", p);
        sem_post(mutex);
        printf("\nProducer %d released Semaphore Mutex \n", getpid());
        sem_post(full);
        printf("\nProducer %d released Semaphore Full \n", getpid());
        sleep(1);
    } //while



    shmdt(buff);
    shmdt(empty);
    shmdt(full);
    shmdt(mutex);
    
    printf("\nProducer / Server Process Exited \n\n");
    return (0);
} //main