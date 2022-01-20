#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include "voiture.h"
#include "session.h"
#include "afficher.h"

//NOMBRE DE TOURS POUR LA FINALE
#define NOMBRE_TOURS_FINALE 45



/******************************
 * DEBUT AFFICHAGE ET COMPAGNIE
*******************************/

void afficherTableau(Voiture *shared_memory, Voiture *copyTableau, Session current_session, sem_t *semaphore, unsigned int tempsMaxCircuit , char *argv) {
    
    while(true){
        system("clear");


        sem_wait(semaphore); //ouvre la porte
        memcpy( &copyTableau, &shared_memory,sizeof(copyTableau) );
        sem_post(semaphore); // fermer la porte

        //TRIE DU TABLEAU + DIFFERENCE DE TEMPS ENTRE VOITURE
        qsort(copyTableau, current_session.total_cars,  sizeof(Voiture), compare); // (__compar_fn_t)
        qsort(copyTableau, current_session.total_cars,  sizeof(Voiture), compare_2); // (__compar_fn_t)


        sortLap(copyTableau , current_session);

        //DISPLAY
        printf("     Meilleur Secteur 1      Meilleur Secteur 2        Meilleur Secteur 3         Meilleur Circuit  \n");
        printf("    ====================    =====================     ====================      ====================\n");
        printf("    Voiture %d -- %.3lfs   Voiture %d -- %.3lfs    Voiture %d -- %.3lfs      Voiture %d -- %.3lfs\n\n", copyTableau[20].best_s1,copyTableau[20].s1, copyTableau[20].best_s2, copyTableau[20].s2, copyTableau[20].best_s3, copyTableau[20].s3,copyTableau[20].best_id, copyTableau[20].best_Circuit);

        printf(" ===============================================================================================\n");
        printf(" |  P  |  VOITURE  |    S1    |    S2    |    S3    |    TOUR    |   GAP   |  Stand  |   out    |\n");
        printf(" |==============================================================================================|\n");
        for (int i = 0; i < current_session.total_cars; i++){
            int p = i + 1;
        printf(" | %2d  |    %2d     |  %.3lf  |  %.3lf  |  %.3lf  |   %.3lf   |  %.3lf  |   %2d    |    %d     |\n", \
                p, copyTableau[i].id, \
                copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                copyTableau[i].best_Circuit,\
                copyTableau[i].lap, \
                copyTableau[i].compteurStand, \
                copyTableau[i].isOut);
        }
        printf(" ===============================================================================================|\n\n");
 
        //SI TOUTES LES VOITURES ONT TERMINER
        if(finished(shared_memory,current_session, tempsMaxCircuit , NOMBRE_TOURS_FINALE)){
            savedFile(copyTableau, argv,current_session);
            break;
        }
        sleep(1);
    }
}

//CALCULE LA DIFFERENCE DE TEMPS ENTRE LES VOITURES(SUR LE TEMPS TOTAL)
void sortLap(Voiture *copyTableau, Session current_session) {
    double difference;
    for (int i = 1; i < current_session.total_cars; i++)
    {   
        if(i == 1){
            copyTableau[i-1].lap = 0;
        }

        if (!copyTableau[i].isOut){

            difference = ( copyTableau[i].best_Circuit - copyTableau[i - 1].best_Circuit );
            copyTableau[i].lap = difference;
        }
        else{
            copyTableau[i].lap = 0;
        }
    }
}

//COMPARER SUR BASE DE LEUR MEILLEUR TEMPS
int compare(const void * a, const void * b)
{
    const Voiture *voitureA = (Voiture *)a;
    const Voiture *voitureB = (Voiture *)b;




    if (voitureA->best_Circuit == voitureB->best_Circuit ){
        return 0;
    }
    else if (voitureA->best_Circuit < voitureB->best_Circuit){
        return -1;
    }
    else{
        return 1;
    }
    // return (int) ( voitureA->best_Circuit - voitureB->best_Circuit );
}

//COMPARER LES VOITURES QUI SONT OUT
int compare_2(const void * a, const void * b){
    const Voiture *voitureA = (Voiture *)a;
    const Voiture *voitureB = (Voiture *)b;

    if (voitureA->isOut && !voitureB->isOut){
        return 1;
    }
    else if (!voitureA->isOut && voitureB->isOut) {
        return -1;
    }
    else{
        return 0;
    }
}

int finished( Voiture *shared_memory, Session current_session, unsigned int tempsMaxCircuit , int nbre_tours_finale) {
    for (int i = 0; i < current_session.total_cars; ++i) {
        if(!shared_memory[i].isOut){
            //SI TOUTES LES VOITURES ONT TOUS ATTEINT LE TEMPS TOTAL
            if (shared_memory[i].tempsTotal >= tempsMaxCircuit) {
                return 1;
            }
                //FINALE : SI TOUTES LES VOITURES ONT PARCOURUE LE NOMBRE DE TOUR
            else if (shared_memory[i].nbre_tours >= nbre_tours_finale) {
                return 1;
            }
        }
    }
    return 0;
}

int savedFile(Voiture *copyTableau,  char *argv , Session current_session) {
  
    char fichiertxt[20] = "../data/";
    strcat(fichiertxt, argv);
    strcat(fichiertxt, ".txt");

    int myFile ;
    ssize_t r;
    mode_t mode = S_IRUSR | S_IWUSR; 
   
    if((myFile = open(fichiertxt , O_CREAT | O_RDWR ,mode)) == -1){
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    char buffer[40] = "";
    char temp[4] = "";
    for (int i = 0; i < current_session.total_cars; i++)
    {   
        sprintf(temp, "%d\n", copyTableau[i].id);
        strcat(buffer,temp);
    }
    write(myFile , buffer, strlen(buffer));
    
    close(myFile);
    return 0;
}


/******************************
 * FIN AFFICHAGE ET COMPAGNIE
*******************************/
