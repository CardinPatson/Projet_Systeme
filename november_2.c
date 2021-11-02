#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMBER_OF_CARS 20
#define MIN 2500 // time generator
#define MAX 4000
#define TEMPS_MAX_STAND 500
#define TEMPS_MIN_STAND 200



int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

typedef struct {

    unsigned int id;

    unsigned int s1;
    unsigned int s2;
    unsigned int s3;

    unsigned int tempsTotal;
    unsigned int tempsTotalComplet;

    unsigned int best_Circuit;

    unsigned int lap;

    unsigned int compteurStand;

    unsigned int out;
} voiture;

voiture *shared_memory; //tableau de structure
voiture copyTableau[NUMBER_OF_CARS];


typedef struct {

    unsigned int best_S1;
    unsigned int best_S2;
    unsigned int best_S3;

    unsigned int best_Circuit;
} meilleur;

meilleur petitTest;




int faireDesTours(int i , int compteur);
unsigned int generateNumber(void);
void afficherTableau(void);
int compare (const void * a, const void * b);
void initVoiture(int i);
void sortLap(void);
unsigned int generateStandStop(void);
unsigned int getLastDigit(unsigned int digit);
bool goStand(unsigned int digit);
bool isOut(int i);
bool savedFile(void);
int lancement(void);
int faireDesToursComplet(int i);
unsigned int compteurDepassement = 0;
void reloadVoiture( int i , int s1 , int s2 , int s3 , int bestS1 , int bestS2 , int bestS3 , int bestCircuit , int lap , int compteurStand) ;


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

    // int compteur = 0;
    
    // while(true){
        lancement();
    //     if(compteur == 5){
    //         break;
    //     }
    //     compteur++;

    // }

    shmdt(shared_memory);

    /********  Supprimer la mémoire partagée  *********/
    shmctl(segment_id, IPC_RMID, NULL);
    
    /***Sauvegarde du classement***/
    savedFile();
    return 0;
}

unsigned int tempsMaxCircuit = 540000;

//OPO : ralentir l'exécution de chaque voiture en les affichant au même moment
//-> Afficher tous au même stade puis faire des tours au même moment


int lancement(void){

     /**********************************************************
     *               Création des fils/voitures               *
     **********************************************************/     
    
    for (int i = 0; i < NUMBER_OF_CARS; ++i)
    {   

        initVoiture(i);//voiture[0]
        /********  échec du fork *********/
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed !");
            exit(EXIT_FAILURE);
        }

        /********  le fils *********/
        if(pid == 0) {

            shared_memory[i].id = numeroVoiture[i]; //chaque voiture à un numéro
            faireDesToursComplet(i ); //1080

            /**TEST**/
            // printf("Nombre de fils %d\n"  ,  i );
            /**FIN TEST**/
            exit(0);
           
        }       
    
        wait(NULL);
       
       
    }

    afficherTableau();
    
    sleep(3);
}

int faireDesTours( int i  , int compteur) {

    unsigned int tour_complet;

    while (shared_memory[i].tempsTotal <= tempsMaxCircuit && !shared_memory[i].out) //time pas dépassée
    {
    srand(time(NULL) + getpid());
        tour_complet = 0;

        /*   ****       S1     ****     */
       shared_memory[i].s1 = generateNumber();
        if (shared_memory[i].s1 <= petitTest.best_S1) {
            petitTest.best_S1 =shared_memory[i].s1;
        }
        shared_memory[i].tempsTotal +=shared_memory[i].s1;
        tour_complet +=shared_memory[i].s1;
        /* *************************************** */

        if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }
        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();
        if (shared_memory[i].s2 <= petitTest.best_S2) {
            petitTest.best_S2 = shared_memory[i].s2;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;
        /* *************************************** */

        if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }

        /*   ****       aller au Stand     ****     */

        // si il va rentre au stand
        shared_memory[i].s3 = generateNumber();
        //si dernier digit == 9 ==> go stand secteur3 + generer le temps sup
        if (goStand(shared_memory[i].s2)) {

    
            unsigned int timeSupplementaire ;
    
            shared_memory[i].compteurStand++;
    
            timeSupplementaire = generateStandStop();
            shared_memory[i].s3 += timeSupplementaire;


            /**TEST**/

            // printf("digit de s2 --> %d" ,shared_memory[i].s2);
            // printf("compteur Stand -->%d\n\n" , shared_memory[i].compteurStand);
            
            
            /**FIN TEST**/
            isOut(i);
            
        }
        /* *************************************** */
        /*   ****       S3     ****     */
        
        if (shared_memory[i].s3 <= petitTest.best_S3) {
            petitTest.best_S3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        /* *************************************** */


        /*   ****       Best Time Circuit     ****     */
        if (tour_complet <  shared_memory[i].best_Circuit) {
            shared_memory[i].best_Circuit = tour_complet;
            petitTest.best_Circuit = tour_complet;
        }
        /* *************************************** */
        
    }
    //afficher tout le tableau avant le sleep
    //si on divisais le 54 000 000  en 5 et toutes les 1 080 000 les voiture se dépasse 
    ///Le temps sera divisais en 5 : creer un tamps max 
    /**TEST = AFFICHE UNE VOITURE A LA FOIS**/
    //Idee au lancement tous les processus font dabord 1 080 000 secondes puis on regenere encore pour
    
    //1.Definir un temps pendant lequel on relève tous les ddépassement de chaque voiture
    /**FIN TEST**/
    reloadVoiture(i , shared_memory[i].s1 ,shared_memory[i].s2,shared_memory[i].s3 ,petitTest.best_S1 , petitTest.best_S2 , petitTest.best_S3 ,shared_memory[i].best_Circuit, shared_memory[i].lap ,shared_memory[i].compteurStand );
    return 0;
}
int faireDesToursComplet(int i){
     faireDesTours(i ,compteurDepassement);
     if(i == 19){
        while(compteurDepassement<10){
            
            for(int j = 0 ; j < NUMBER_OF_CARS -1 ; j++){
                faireDesTours(j , compteurDepassement);
            }
            sleep(1);
            afficherTableau();
            compteurDepassement++;      
        }
       
        printf("compteur --> %d" , compteurDepassement );
     }
     
}



unsigned int generateNumber(void)
{
    return (rand()%(MAX-MIN+1)+MIN);
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

    petitTest.best_S1 = MAX;
    petitTest.best_S2 = MAX;
    petitTest.best_S3 = MAX;

    shared_memory[i].best_Circuit = 3 * MAX;
    shared_memory[i].tempsTotal = 0;

    shared_memory[i].lap = 0;
    shared_memory[i].compteurStand = 0;
    shared_memory[i].out = false;
}

void reloadVoiture( int i , int s1 , int s2 , int s3 , int bestS1 , int bestS2 , int bestS3 , int bestCircuit , int lap , int compteurStand) {
    shared_memory[i].s1 += s1;
    shared_memory[i].s2 += s2;
    shared_memory[i].s3 += s3;

    petitTest.best_S1 += bestS1;
    petitTest.best_S2 += bestS2;
    petitTest.best_S3 += bestS3;

    shared_memory[i].best_Circuit += bestCircuit;

    shared_memory[i].lap += lap;
    shared_memory[i].compteurStand = compteurStand;
}

// void reloadVoiture(int i  , voiture)



void sortLap(void) {

    unsigned int difference;

    for (int i = 1; i < NUMBER_OF_CARS; i++)
    {
        difference = ( copyTableau[i].best_Circuit - copyTableau[i - 1].best_Circuit );

        copyTableau[i].lap = difference;
    }
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
    return (rand() % (TEMPS_MAX_STAND - TEMPS_MIN_STAND + 1) + TEMPS_MIN_STAND);
}

bool isOut(int i) {
    if(shared_memory[i].compteurStand > 5) {
        shared_memory[i].out = true;
    }
}


void afficherTableau() {
    
    system("clear");
    // copy of array    

    memcpy( copyTableau, shared_memory, sizeof(copyTableau) );
    
    //trier Tableau;
    qsort( copyTableau, NUMBER_OF_CARS, sizeof(voiture), compare );
    sortLap();

    
    printf("\n\tMeilleurs temps par tour complet\n");
    printf(" ==============================================================================================================\n");
    printf(" |     ID   |      s1     |      s2     |      s3     |     Tour    |     LAP     |   Stand  |   Temps Total  |\n");
    printf(" |============================================================================================================|\n");
    
    for (int i = 0; i < NUMBER_OF_CARS; i++){
        printf(" |     %2d   |    %5d    |    %5d    |    %5d    |    %6d    |    %5d    |    %2d    | %5d       | \n", \
                copyTableau[i].id, \
                copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                copyTableau[i].best_Circuit,\
                copyTableau[i].lap, \
                copyTableau[i].compteurStand , copyTableau[i].tempsTotalComplet);
    }
    printf(" =============================================================================================================\n\n");

    printf("bs1: %d, bs2: %d, bs3: %d et b_circuit %d\n\n", petitTest.best_S1, petitTest.best_S2, petitTest.best_S3, petitTest.best_Circuit );


    
}


bool savedFile(void) {

    FILE *fichier = fopen("classement.txt", "w");
    if (fichier == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        fprintf(fichier, "%d\n", copyTableau[i].id);
    }
    fclose(fichier);

    return true;
}
