#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include "libs/voiture.h"
#include "libs/session.h"
#include "libs/course.h"
#include "libs/afficher.h"
#include "libs/share.h"

#define NUMBER_OF_CARS 20

//INTERVALLE DE TEMPS POUR LES SECTEURS
#define MIN 25.00
#define MAX 40.00


int lancement(Voiture *shared_memory,Session current_session, sem_t *semaphore, char *argv, unsigned int* numeroVoiture , unsigned int* qualifiedCars);
void initVoiture(int i  , Voiture *shared_memory) ;
void initBest( Voiture *shared_memory);



int main(int argc , char *argv[])
{
    
    unsigned int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    unsigned int qualifiedCars[NUMBER_OF_CARS];

    Shared_data data_shared;
    Session current_session;

    define_session(argc , argv, numeroVoiture, qualifiedCars, &current_session);
    /***************************************************
   *           Création de la mémoire partagée        *
   ****************************************************/
    Voiture *shared_memory;
    shared_memory = create_shared_memory(&data_shared, current_session.total_cars);


    /********  Initialisation du sémaphore  *********/
    init_semaphore(&data_shared);


    /********  Lancement de la course  *********/
    lancement(shared_memory, current_session, data_shared.semaphore, argv[1], numeroVoiture, qualifiedCars);


    /********  Détachament des segments de mémoire  *********/
    detach_shared_memory(shared_memory , data_shared.segment_id);


    /********  Destruction des sémaphores  *********/
    destroy_semaphore(&data_shared); 
   
    exit(EXIT_SUCCESS);
}


/****************************************
 * DEBUT LANCEMENT ET COMPAGNIE
*****************************************/
int lancement(Voiture *shared_memory,Session current_session, sem_t *semaphore, char *argv, unsigned int* numeroVoiture , unsigned int* qualifiedCars)
{


    //PERMET DE STOCKER LES MEILLEURS DANS UNE CASE SUPPLEMENTAIRE DE LA MEMOIRE PARTAGEE
    initBest(shared_memory);
    Voiture copyTableau[current_session.total_cars + 1];

    // CREATION DES FILS
    for (int i = 0; i < current_session.total_cars; ++i)
    {
        initVoiture(i, shared_memory);
        /********  échec du fork *********/
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed !");
            exit(EXIT_FAILURE);
        }

        /********  les fils *********/
        if(pid == 0) {
    
            //POUR LA FINALE
            if (strcmp(argv, "FINALE") == 0){
                
                shared_memory[i].id = numeroVoiture[i];
                finale(shared_memory, semaphore, i);
                exit(EXIT_SUCCESS);
            }

            else{
                shared_memory[i].id = numeroVoiture[i]; //attribution numéro pour chaque voiture
                faireDesTours(shared_memory, semaphore, i ,current_session.session_time);
                exit(EXIT_SUCCESS);
            }
        }
    }

    //AFFICHAGE PERE
    afficherTableau(shared_memory,copyTableau, current_session, semaphore, current_session.session_time , argv);
    exit(EXIT_SUCCESS);
}

/** INITIALISATION DU MEILLEUR : CELUI DANS LA CASE SUPP **/
void initBest(Voiture *shared_memory) {
    shared_memory[20].s1 = MAX;
    shared_memory[20].s2 = MAX;
    shared_memory[20].s3 = MAX;
    shared_memory[20].best_Circuit = 3*MAX;
}

/** INITIALISATION DUNE VOITURE **/
void initVoiture(int i  , Voiture *shared_memory) {
    shared_memory[i].s1 = 0.00;
    shared_memory[i].s2 = 0.00;
    shared_memory[i].s3 = 0.00;
    shared_memory[i].best_Circuit = 3.00 * MAX;
    shared_memory[i].tempsTotal = 0.00;
    shared_memory[i].lap = 0.00;
    shared_memory[i].compteurStand = 0;
    shared_memory[i].isOut = false;
    shared_memory[i].isFinished = false;
    shared_memory[i].nbre_tours = 0;
}
/****************************************
 * FIN LANCEMENT ET COMPAGNIE
*****************************************/
//HOW TO COMPILE 

//for one file 
//clang [-E or -S] main.c 
 


 //Segmentation fault lors de la manipulation des pointeurs

 // Si on a un tableau et on veut passer ce tableua à une fonction on est oblige de mentionner la logueur du tableua 
 //Si non on aura des erreurs de segmentation comme quoi on essaie d'acceder à de la mémoire qu'on ne doit pas 