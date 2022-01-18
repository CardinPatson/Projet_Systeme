#ifndef __VOITURE_H_
#define __VOITURE_H_
#include <semaphore.h>
typedef struct Voiture {

   unsigned int id;
    double s1;
    double s2;
    double s3;
    double tempsTotal;
    double best_Circuit;
    double lap;
    unsigned int compteurStand;
    unsigned int isOut;
    unsigned int nbre_tours;
    bool isFinished ;
} Voiture;

typedef struct Shared_data {
    int segment_id;
    int sem_shm_id;
    sem_t *semaphore;
} Shared_data;

#endif