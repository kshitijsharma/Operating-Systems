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
extern int errno;

#define true 1
#define SIZE 10   /* size of the shared buffer*/
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
    segid = shmget(315, SIZE * sizeof(int), IPC_EXCL | SHMPERM);
    empty_id = shmget(316, sizeof(sem_t),  IPC_EXCL | SHMPERM);
    full_id = shmget(317, sizeof(sem_t), IPC_EXCL | SHMPERM);
    mutex_id = shmget(318, sizeof(sem_t),  IPC_EXCL | SHMPERM);
    buff = shmat(segid, (char *)0, 0);
    empty = shmat(empty_id, (char *)0, 0);
    full = shmat(full_id, (char *)0, 0);
    mutex = shmat(mutex_id, (char *)0, 0);
    
    printf("\nConsumer / Client Process Started \n");
    
    sleep(2);
    
    unsigned int i = 0,c = 0;
    while (true)
    {
        if (i >= SIZE)
        {
            printf("\nConsumer %d exited \n", getpid());
            exit(1);
        }

        printf("\nConsumer %d trying to aquire Semaphore Full \n", getpid());
        sem_wait(full);
        printf("\nConsumer %d successfully aquired Semaphore Full \n", getpid());
        printf("\nConsumer %d trying to aquire Semaphore Mutex \n", getpid());
        sem_wait(mutex);
        printf("\nConsumer %d successfully aquired Semaphore Mutex\n", getpid());
        printf("\nConsumer %d Consumed Item [ %d ] \n", getpid(), buff[c]);
        buff[c] = -1;
        c++;
        printf("\nItems in Buffer %d \n", SIZE - c);
        i++;
        sem_post(mutex);
        printf("\nConsumer %d released Semaphore Mutex \n", getpid());
        sem_post(empty);
        printf("\nConsumer %d released Semaphore Empty \n", getpid());
        sleep(1);
    } //while

    sleep(2);
    shmdt(buff);
    shmdt(empty);
    shmdt(full);
    shmdt(mutex);
    shmctl(segid, IPC_RMID, NULL);
    semctl(empty_id, 0, IPC_RMID, NULL);
    semctl(full_id, 0, IPC_RMID, NULL);
    semctl(mutex_id, 0, IPC_RMID, NULL);
    sem_destroy(empty);
    sem_destroy(full);
    sem_destroy(mutex);
    printf("\nConsumer / Client process exited \n\n");
    return (0);
} //main