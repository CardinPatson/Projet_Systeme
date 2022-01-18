#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
// #include <sys/shm.h>
#include <semaphore.h>
#include "voiture.h"
#include "session.h"
#include "afficher.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
        sortLap(copyTableau , current_session);

        //DISPLAY
        printf("\n\tMeilleurs temps par tour complet\n");
        printf(" =====================================================================================\n");
        printf(" |  P  |  VOITURE  |    S1    |    S2    |    S3    |    TOUR    |   GAP   |  Stand  |\n");
        printf(" |===================================================================================|\n");
        for (int i = 0; i < current_session.total_cars; i++){
            int p = i + 1;
        printf(" | %2d  |    %2d     |  %.3lf  |  %.3lf  |  %.3lf  |   %.3lf   |  %.3lf  |   %2d    |\n", \
                p, copyTableau[i].id, \
                copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                copyTableau[i].best_Circuit,\
                copyTableau[i].lap, \
                copyTableau[i].compteurStand);
        }
        printf(" =====================================================================================\n\n");

        printf("bs1: %.3lf, bs2: %.3lf, bs3: %.3lf et b_circuit %.3lf\n", copyTableau[20].s1, copyTableau[20].s2, copyTableau[20].s3, copyTableau[20].best_Circuit);

        //SI TOUTES LES VOITURES ONT TERMINER
        if(finished(shared_memory,current_session, tempsMaxCircuit , NOMBRE_TOURS_FINALE)){
            savedFile(copyTableau, argv,current_session);
            break;
        }
        sleep(1);
    }
}


void sortLap(Voiture *copyTableau, Session current_session) {
    //DIFFERENCE DE TEMPS ENTRE LES VOITURES(SUR LE TEMPS TOTAL)
    double difference;
    for (int i = 1; i < current_session.total_cars; i++)
    {
        difference = ( copyTableau[i].best_Circuit - copyTableau[i - 1].best_Circuit );
        copyTableau[i].lap = difference;
    }
}


int compare(const void * a, const void * b)
{
    const Voiture *voitureA = (Voiture *)a;
    const Voiture *voitureB = (Voiture *)b;
    if (voitureA->best_Circuit == voitureB->best_Circuit){
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

int finished( Voiture *shared_memory, Session current_session, unsigned int tempsMaxCircuit , int nbre_tours_finale) {
    for (int i = 0; i < current_session.total_cars; ++i) {

        //SI TOUTES LES VOITURES ONT TOUS ATTEINT LE TEMPS TOTAL
        if (shared_memory[i].tempsTotal >= tempsMaxCircuit) {
            return 1;
        }
            //FINALE : SI TOUTES LES VOITURES ONT PARCOURUE LE NOMBRE DE TOUR
        else if (shared_memory[i].nbre_tours >= nbre_tours_finale) {
            return 1;
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
   
    if((myFile = open(fichiertxt , O_CREAT,mode)) == -1){
            perror("open failed");
            exit(EXIT_FAILURE);
    }
    
    if (close(myFile) < 0) 
    {
        perror("Erreur lors du close.");
        exit(EXIT_FAILURE);  
    }

    if ((myFile=open(fichiertxt,O_RDWR,mode)) < 0)
    {
        perror("Erreur lors du Open pour écrire dans le fichier");
        exit(EXIT_FAILURE);    
    }
    char buffer[50];
    char temp[4];
    for (int i = 0; i < current_session.total_cars; ++i)
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
