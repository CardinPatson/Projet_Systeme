#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> 
#include <sys/ipc.h>
#include <sys/types.h>
#include <semaphore.h>
#include "voiture.h"
#include "course.h"


//INTERVALLE DE TEMPS POUR LES SECTEURS
#define MIN 25.00
#define MAX 40.00

//INTERVALLE DE TEMPS POUR LES STANDS
#define TEMPS_MAX_STAND 25.00
#define TEMPS_MIN_STAND 12.00

//NOMBRE DE TOURS POUR LA FINALE
#define NOMBRE_TOURS_FINALE 45


/****************************************
 * DEBUT  COURSE ET COMPAGNIE
*****************************************/
int faireDesTours( Voiture *shared_memory,  sem_t *semaphore, int i , unsigned int tempsMaxCircuit ) {

    double tour_complet; //TEMPS SUR S1+S2+S3
    srand(time(NULL) + getpid());


    while (shared_memory[i].tempsTotal <= tempsMaxCircuit && !shared_memory[i].isOut) //time pas dépassée
    {
        tour_complet = 0;

        sem_wait(semaphore);
        /*   ****       S1     ****     */
        shared_memory[i].s1 = generateNumber();

        //SI IL A UN TEMPS < AU MEILLEUR
        if (shared_memory[i].s1 < shared_memory[20].s1) {
            shared_memory[20].s1 = shared_memory[i].s1;
        }

        shared_memory[i].tempsTotal += shared_memory[i].s1;
        tour_complet += shared_memory[i].s1;
        /* *************************************** */
        sem_post(semaphore);


        sem_wait(semaphore);
        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();

        if (shared_memory[i].s2 < shared_memory[20].s2) {
            shared_memory[20].s2 = shared_memory[i].s2;
        }

        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;
        /* *************************************** */
        sem_post(semaphore);


        sem_wait(semaphore);

        /*   ****       aller au Stand     ****     */
        shared_memory[i].s3 = generateNumber();

        //si dernier digit == 9 ==> go stand secteur3 + generer le temps sup
        if (goStand(shared_memory[i].s2)) {
            double timeSupplementaire ;
            shared_memory[i].compteurStand++;
            timeSupplementaire = generateStandStop();
            shared_memory[i].s3 += timeSupplementaire;

            //VERIFIE SIL NA PAS DEPASSE LE NOMBRE LIMITE DE PASSAGE AU STAND
            goOut(i , shared_memory);
        }
        usleep(80);
        /* *************************************** */

        /*   ****       S3     ****     */
        if (shared_memory[i].s3 < shared_memory[20].s3) {
            shared_memory[20].s3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        /* *************************************** */

        /*   ****       Best Time Circuit de toutes les voitures     ****     */
        //SI IL FAIT UN TPS < AU TPS DANS LA CASE SUPP
        if (tour_complet < shared_memory[20].best_Circuit && !shared_memory[i].isOut) {
            shared_memory[20].best_Circuit = tour_complet;
        }
        /*   ****       Best Time Circuit de la voiture actuelle     ****     */
        if (tour_complet < shared_memory[i].best_Circuit && !shared_memory[i].isOut) {
            shared_memory[i].best_Circuit = tour_complet;
        }
        
        sem_post(semaphore);

        sleep(1);
        /* *************************************** */
    }
    return 0;
}

int finale( Voiture *shared_memory, sem_t *semaphore, int i ){

    double tour_complet;
    srand(time(NULL) + getpid());
    while (shared_memory[i].nbre_tours <= NOMBRE_TOURS_FINALE && !shared_memory[i].isOut) //time pas dépassée
    {
        tour_complet = 0;

        sem_wait(semaphore);
        /*   ****       S1     ****     */
        shared_memory[i].s1 = generateNumber();

        //SI IL A UN TEMPS < AU MEILLEUR
        if (shared_memory[i].s1 < shared_memory[20].s1) {
            shared_memory[20].s1 = shared_memory[i].s1;
        }

        shared_memory[i].tempsTotal += shared_memory[i].s1;
        tour_complet += shared_memory[i].s1;
        /* *************************************** */
        sem_post(semaphore);


        sem_wait(semaphore);
        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();

        if (shared_memory[i].s2 < shared_memory[20].s2) {
            shared_memory[20].s2 = shared_memory[i].s2;
        }

        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;
        /* *************************************** */
        sem_post(semaphore);


        sem_wait(semaphore);

        /*   ****       aller au Stand     ****     */
        shared_memory[i].s3 = generateNumber();

        //si dernier digit == 9 ==> go stand secteur3 + generer le temps sup
        if (goStand(shared_memory[i].s2)) {
            double timeSupplementaire ;
            shared_memory[i].compteurStand++;
            timeSupplementaire = generateStandStop();
            shared_memory[i].s3 += timeSupplementaire;

            //VERIFIE SIL NA PAS DEPASSE LE NOMBRE LIMITE DE PASSAGE AU STAND
            goOut(i , shared_memory);
        }
        usleep(80);
        /* *************************************** */

        /*   ****       S3     ****     */
        if (shared_memory[i].s3 < shared_memory[20].s3) {
            shared_memory[20].s3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        /* *************************************** */

        /*   ****       Best Time Circuit de toutes les voitures     ****     */
        //SI IL FAIT UN TPS < AU TPS DANS LA CASE SUPP

        if (tour_complet < shared_memory[20].best_Circuit && !shared_memory[i].isOut) {
            shared_memory[20].best_Circuit = tour_complet;
        }

        /*   ****       Best Time Circuit de la voiture actuelle     ****     */
        if (tour_complet < shared_memory[i].best_Circuit && !shared_memory[i].isOut) {
            shared_memory[i].best_Circuit = tour_complet;
        }
        shared_memory[i].nbre_tours++;
        sem_post(semaphore);
        sleep(1);
    }

    return 0 ;
}

//GENERATION ALEATOIRE DE TEMPS POUR UN SECTEUR
double generateNumber(void) {
    double r = random();
    return (MAX-MIN)*( r / ((double)RAND_MAX + 1 )) + MIN;
}

//GENERATION ALEATOIRE DE TEMPS POUR UN STAND
double generateStandStop(void){
    double r = random();
    return (TEMPS_MAX_STAND-TEMPS_MIN_STAND)*( r / ((double)RAND_MAX + 1 )) + TEMPS_MIN_STAND;
}


bool goStand(unsigned int digit) {

    //STAND SI LE DERNIER DIGIT DE Q3 --> 9
    if(digit%10 == 9) {
        return true;
    }
    else {
        return false;
    }
}

void goOut(int i,  Voiture *shared_memory) {

    //SI JATTEINT LA LIMITE DE PASSAGE AU STAND : 19
    if(shared_memory[i].compteurStand > 7) {
        shared_memory[i].isOut = true;
    }
}

/****************************
 * FIN COURSE ET COMPAGNIE
*****************************/
