#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


#define NUMBER_OF_CARS 20
#define MIN 25 // time generator
#define MAX 40
#define TEMPS_MAX_STAND 25
#define TEMPS_MIN_STAND 12


int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};


typedef struct {

    unsigned int id;

    unsigned int s1;
    unsigned int s2;
    unsigned int s3;

    unsigned int tempsTotal;

    unsigned int best_Circuit;

    unsigned int lap;

    unsigned int compteurStand;

    unsigned int isOut;
} voiture;

voiture *shared_memory; //tableau de structure
voiture copyTableau[NUMBER_OF_CARS + 1];



int faireDesTours(int i);
unsigned int generateNumber(void);
void afficherTableau(void);
unsigned int compare (const void * a, const void * b);
void initVoiture(int i);
void sortLap(void);
unsigned int generateStandStop(void);
bool goStand(unsigned int digit);

void lancement(void);
void initBest(void);




int main(void)
{
    /***************************************************
   *           Création de la mémoire partagée        *
   ****************************************************/
    int segment_id = shmget(IPC_PRIVATE, sizeof(voiture) * (NUMBER_OF_CARS +1), 0666 | IPC_CREAT);
    if (segment_id == -1) {
        perror("shmget() failed !");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(segment_id, NULL, 0);
    if (shared_memory == (void *) (-1)) {
        perror("shmat() failed !");
        exit(EXIT_FAILURE);
    }
    /* ***********-----------------********** */

    lancement();

    /* delete shered memory car */
    shmdt(shared_memory);
    shmctl(segment_id, IPC_RMID, NULL);

    exit(EXIT_SUCCESS);
}


void lancement(void)
{
    initBest();

    int course_active = true;
    int counter = 0;

    /**********************************************************
    *               Création des fils/voitures               *
    **********************************************************/
    // Création d'un fils
    int i = 0;
    pid_t pid;
    for (i; i < NUMBER_OF_CARS; i++) // On fork autant de fois qu'il y a de voitures dans la course
    {
        pid = fork();
        if (pid == 0){
            break; // empeche de lancer 20^20 processus
        }
    }
    switch (pid)
    {
        case -1:
            perror("Erreur lors du fork !");
            exit(EXIT_FAILURE);

        case 0:
            while (course_active) {
                initVoiture(i);
                shared_memory[i].id = numeroVoiture[i];
                faireDesTours(i);
                sleep(1);
            }

        default:
            while (course_active) {
                afficherTableau();
                sleep(1);
                counter++;

                if (counter == 10) {
                    course_active = false;
                    exit(EXIT_SUCCESS);
                }
            }
    }
}

int faireDesTours( int i ) {

    unsigned int tour_complet;

    srand(time(NULL) + getpid());

    int counter = 0;

    while (counter < 55) //time pas dépassée
    {
        tour_complet = 0;

        /*   ****       S1     ****     */
        shared_memory[i].s1 = generateNumber();
        if (shared_memory[i].s1 < shared_memory[20].s1) {
            shared_memory[20].s1 = shared_memory[i].s1;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s1;
        tour_complet += shared_memory[i].s1;
        /* *************************************** */


        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();
        if (shared_memory[i].s2 < shared_memory[20].s2) {
            shared_memory[20].s2 = shared_memory[i].s2;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;
        /* *************************************** */



        /*   ****       aller au Stand     ****     */

        // si il va rentre au stand
        shared_memory[i].s3 = generateNumber();

        //si dernier digit == 9 ==> go stand secteur3 + generer le temps sup
        if (goStand(shared_memory[i].s2)) {

            unsigned int timeSupplementaire ;

            shared_memory[i].compteurStand++;

            timeSupplementaire = generateStandStop();
            shared_memory[i].s3 += timeSupplementaire;

        }
        /* *************************************** */

        /*   ****       S3     ****     */
        shared_memory[i].s3 = generateNumber();
        if (shared_memory[i].s3 < shared_memory[20].s3) {
            shared_memory[20].s3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        /* *************************************** */


        /*   ****       Best Time Circuit     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
            shared_memory[i].best_Circuit = tour_complet;
            shared_memory[20].best_Circuit = tour_complet;
        }
        /* *************************************** */

        counter++;
    }
    sleep(2);
    return 0;
}


unsigned int generateNumber(void)
{
    return rand() * clock() % (MAX-MIN+1) + MIN;
}


unsigned int compare(const void * a, const void * b)
{
    voiture *voitureA = (voiture *)a;
    voiture *voitureB = (voiture *)b;

    return ( voitureA->best_Circuit - voitureB->best_Circuit );
}


void initVoiture(int i) {
    shared_memory[i].s1 = 0;
    shared_memory[i].s2 = 0;
    shared_memory[i].s3 = 0;

    shared_memory[i].best_Circuit = 3 * MAX;
    shared_memory[i].tempsTotal = 0;

    shared_memory[i].lap = 0;
    shared_memory[i].compteurStand = 0;
    shared_memory[i].isOut = false;
}


void initBest(void) {
    shared_memory[20].s1 = MAX;
    shared_memory[20].s2 = MAX;
    shared_memory[20].s3 = MAX;
}



bool goStand(unsigned int digit) {

    if(digit%10 == 9) {
        return true;
    }
    else {
        return false;
    }
}

unsigned int generateStandStop(void){
    return rand() * clock() % (TEMPS_MAX_STAND - TEMPS_MIN_STAND + 1) + TEMPS_MIN_STAND;
}



void sortLap(void) {

    unsigned int difference;

    for (int i = 1; i < NUMBER_OF_CARS; i++)
    {
        difference = ( copyTableau[i].best_Circuit - copyTableau[i - 1].best_Circuit );

        copyTableau[i].lap = difference;
    }
}


void afficherTableau() {

    system("clear");

    memcpy( copyTableau, shared_memory, sizeof(copyTableau) );

    qsort(copyTableau, (NUMBER_OF_CARS), sizeof(voiture), (__compar_fn_t) compare);
    sortLap();

    printf("\n\tMeilleurs temps par tour complet\n");
    printf(" =============================================================================================\n");
    printf(" |     ID   |      s1     |      s2     |      s3     |     Tour    |     LAP     |   Stand  |\n");
    printf(" |===========================================================================================|\n");

    for (int i = 0; i < NUMBER_OF_CARS; i++){
        printf(" |     %2d   |    %5d    |    %5d    |    %5d    |    %5d    |    %5d    |    %2d    | %5d\n", \
                copyTableau[i].id, \
                copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                copyTableau[i].best_Circuit,\
                copyTableau[i].lap, \
                copyTableau[i].compteurStand, copyTableau[i].tempsTotal);
    }
    printf(" =============================================================================================\n\n");
    printf("bs1: %d, bs2: %d, bs3: %d et b_circuit %d\n", copyTableau[20].s1, copyTableau[20].s2, copyTableau[20].s3, copyTableau[20].best_Circuit);

    sleep(2);
}
