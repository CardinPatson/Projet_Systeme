#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> 
#include "voiture.h"
#include "session.h"
#include "share.h"
 
Voiture* create_shared_memory(Shared_data *data_shared, unsigned int total_cars){
    Voiture *shared_memory;
    data_shared->segment_id = shmget(IPC_PRIVATE, (total_cars+1)*sizeof(struct Voiture), 0666 | IPC_CREAT);
    if (data_shared->segment_id == -1) {
        perror("shmget() failed !");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(data_shared->segment_id, NULL, 0);
    if (shared_memory == (void *) (-1)) {
        perror("shmat() failed !");
        exit(EXIT_FAILURE);
    }
    return shared_memory;
}
void init_semaphore(Shared_data *data_shared){
    data_shared->sem_shm_id = shmget(IPC_PRIVATE, sizeof(sem_t), 0600 | IPC_CREAT);
    if (data_shared->sem_shm_id == -1) {
        perror("shmget failed !");
        exit(EXIT_FAILURE);
    }
    data_shared->semaphore = shmat(data_shared->sem_shm_id, NULL, 0);
    if (data_shared->semaphore == (void *) (-1)) {
        perror("shmat failed !");
        exit(EXIT_FAILURE);
    }

    sem_init(data_shared->semaphore, 1, 1);
}
void detach_shared_memory(Voiture *shared_memory ,int segment_id){
     
    /********  Détachament des segments de mémoire  *********/
    shmdt(shared_memory);

    /********  Supprimer la mémoire partagée  *********/
    shmctl(segment_id, IPC_RMID, NULL);
}

void destroy_semaphore(Shared_data *data_shared){
     /********  Destruction des sémaphores  *********/
    
    sem_destroy(data_shared->semaphore);
    
    shmdt(data_shared->semaphore);
    shmctl(data_shared->sem_shm_id, IPC_RMID, NULL);
}