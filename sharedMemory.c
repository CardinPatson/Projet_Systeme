#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMBER_OF_CARS 20
#define MIN 25000 // time generator
#define MAX 40000

int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

typedef struct {

    unsigned int id;

    unsigned int s1;
    unsigned int s2;
    unsigned int s3;


    unsigned int tempsTotal;


    unsigned int best_S1;
    unsigned int best_S2;
    unsigned int best_S3;

    unsigned int best_Circuit;
} voiture;

voiture *shared_memory;

voiture copyTableau[NUMBER_OF_CARS];



int faireDesTours(int i);
unsigned int generateNumber(void);
void afficherTableau(void);
int compare (const void * a, const void * b);
void initVoiture(int i);




int main(void)
{
    /***************************************************
    *           Création de la mémoire partagée        *
    ****************************************************/
    int segment_id = shmget(IPC_PRIVATE, sizeof(voiture) * NUMBER_OF_CARS, 0666 | IPC_CREAT);
    if (segment_id == -1) {
        perror("shmget() failed !");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(segment_id, NULL, 0);
    if (shared_memory == (void *) (-1)) {
        perror("shmat() failed !");
        exit(EXIT_FAILURE);
    }


    /**********************************************************
     *               Création des fils/voitures               *
     **********************************************************/
    int i;
    pid_t pid = 0;
    for (i = 0; i < NUMBER_OF_CARS; i++) {
        pid = fork();
        if (pid == 0)
            break;
    }

    switch (pid) {

        case -1:
            /********  échec du fork *********/
            perror("fork failed !");
            exit(EXIT_FAILURE);

        case 0:
            shared_memory[i].id = numeroVoiture[i]; //chaque voiture à un numéro
            faireDesTours(i); //5400000

            exit(EXIT_SUCCESS);

        default:
            /********  Appel de la fonction display qui va afficher les données  *********/
            // copy of array
            memcpy( copyTableau, shared_memory, sizeof(copyTableau) );

            //trier Tableau;
            qsort( copyTableau, NUMBER_OF_CARS, sizeof(voiture), compare );

            afficherTableau();

            /********  wait for children to finish  *********/
            for (int j = 0; j < NUMBER_OF_CARS; j++) {
                wait(NULL);
            }
            
    }


    /********  Détachament des segments de mémoire  *********/
    shmdt(shared_memory);

    /********  Supprimer la mémoire partagée  *********/
    shmctl(segment_id, IPC_RMID, NULL);


    exit(EXIT_SUCCESS);
}



unsigned int tempsMaxCircuit = 5400000;

int faireDesTours( int i ) {

    initVoiture(i);


    unsigned int tour_complet;


    while ( shared_memory[i].tempsTotal <= tempsMaxCircuit ) //time pas dépassée
    {
        tour_complet = 0;

        srand(time(NULL) + getpid());

        /*   ****       S1     ****     */
        shared_memory[i].s1 = generateNumber();
        if (shared_memory[i].s1 < shared_memory[i].best_S1) {
            shared_memory[i].best_S1 = shared_memory[i].s1;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s1;
        tour_complet += shared_memory[i].s1;
        /* *************************************** */

        if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }
        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();
        if (shared_memory[i].s2 < shared_memory[i].best_S2) {
            shared_memory[i].best_S2 = shared_memory[i].s2;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;
        /* *************************************** */

        if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }
        /*   ****       S3     ****     */
        shared_memory[i].s3 = generateNumber();
        if (shared_memory[i].s3 < shared_memory[i].best_S3) {
            shared_memory[i].best_S3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        /* *************************************** */


        /*   ****       Best Time Circuit     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
            shared_memory[i].best_Circuit = tour_complet;
        }
        /* *************************************** */

        if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }
    }

    return 0;
}



unsigned int generateNumber(void)
{
    return rand()%(MAX-MIN+1)+MIN;
}


void afficherTableau(void) {
    printf("\n\tMeilleurs temps par tour complet\n");
    printf("====================================================================\n");
    printf("|     ID   |      s1     |      s2     |      s3     |     Tour    |\n");
    printf("|==================================================================|\n");
    for (int i = 0; i < NUMBER_OF_CARS; i++){
        printf("|     %2d   |    %4d    |    %4d    |    %4d    |    %4d    |\n", \
                copyTableau[i].id, \
                copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                copyTableau[i].best_Circuit );
    }
    printf("====================================================================\n\n");
}



int compare(const void * a, const void * b)
{
    voiture *voitureA = (voiture *)a;
    voiture *voitureB = (voiture *)b;

    return ( voitureA->best_Circuit - voitureB->best_Circuit );
}


void initVoiture(int i) {
    shared_memory[i].s1 = 0;
    shared_memory[i].s2 = 0;
    shared_memory[i].s3 = 0;

    shared_memory[i].best_S1 = MAX;
    shared_memory[i].best_S2 = MAX;
    shared_memory[i].best_S3 = MAX;

    shared_memory[i].best_Circuit = 3 * MAX;
    shared_memory[i].tempsTotal = 0;
}
