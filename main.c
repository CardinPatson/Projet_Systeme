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

    bool isFinished ; 

} voiture;

typedef struct{

    char file_name[20];
    unsigned int session_time;
    unsigned int total_cars;


}Session;

Session current_session;

voiture *shared_memory; 
voiture copyTableau[NUMBER_OF_CARS + 1];



int faireDesTours( int i , int tempsMaxCircuit );
unsigned int generateNumber(void);
void afficherTableau(int tempsMaxCircuit , char **argv);
unsigned int compare (const void * a, const void * b);
void initVoiture(int i);
void sortLap(void);
unsigned int generateStandStop(void);
bool goStand(unsigned int digit);
void goOut(int i);
int lancement(int tempsMaxCircuit , char **argv , bool course_fini);
int finished(int tempsMaxCircuit , char **argv);
void initBest(void) ;
void define_session(int argc, char *argv[]);
bool savedFile(char *argv[]);
bool test_finish(char **argv);
void check_course(char course[]);



int main(int argc , char *argv[])
{
    /***************************************************
   *           Création de la mémoire partagée        *
   ****************************************************/
    int segment_id = shmget(IPC_PRIVATE, sizeof(voiture) * NUMBER_OF_CARS+1, 0666 | IPC_CREAT);
    bool course_fini = false;


    if (segment_id == -1) {
        perror("shmget() failed !");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(segment_id, NULL, 0);
    if (shared_memory == (void *) (-1)) {
        perror("shmat() failed !");
        exit(EXIT_FAILURE);
    }
    define_session(argc , argv);
    lancement(current_session.session_time , argv , course_fini);

   

    
  

    shmdt(shared_memory);

    /********  Supprimer la mémoire partagée  *********/
    shmctl(segment_id, IPC_RMID, NULL);
    
    
    return 0;
}

int lancement(int tempsMaxCircuit , char **argv , bool course_fini)
{   initBest();
    /**********************************************************
    *               Création des fils/voitures               *
    **********************************************************/
    for (int i = 0; i < NUMBER_OF_CARS; ++i)
    {
        initVoiture(i);

        /********  échec du fork *********/
        pid_t pid = fork();

        int pids[20];

        if (pid == -1) {
            perror("fork failed !");
            exit(EXIT_FAILURE);
        }

        /********  le fils *********/
        if(pid == 0) {
            shared_memory[i].id = numeroVoiture[i]; //attribution numéro pour chaque voiture

            faireDesTours(i ,  current_session.session_time);

            exit(EXIT_SUCCESS);

        }
        
    }

    afficherTableau(current_session.session_time , argv); 
    exit(EXIT_SUCCESS);
    
    
}


int faireDesTours( int i , int tempsMaxCircuit ) {

    unsigned int tour_complet;

    

    srand(time(NULL) + getpid());
    while (shared_memory[i].tempsTotal <= tempsMaxCircuit && !shared_memory[i].isOut) //time pas dépassée
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

        // if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        // { 
        //     break;
        // }
        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();
        if (shared_memory[i].s2 < shared_memory[20].s2) {
            shared_memory[20].s2 = shared_memory[i].s2;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;
        /* *************************************** */

        // if (shared_memory[i].tempsTotal >= tempsMaxCircuit)
        // {
        //     break;
        // }

        /*   ****       aller au Stand     ****     */

        // si il va rentre au stand
        shared_memory[i].s3 = generateNumber();
        //si dernier digit == 9 ==> go stand secteur3 + generer le temps sup
        if (goStand(shared_memory[i].s2)) {


            unsigned int timeSupplementaire ;

            shared_memory[i].compteurStand++;

            timeSupplementaire = generateStandStop();
            shared_memory[i].s3 += timeSupplementaire;

            goOut(i);

        }
        sleep(1);
        /* *************************************** */
        /*   ****       S3     ****     */

         if (shared_memory[i].s3 < shared_memory[20].s3) {
            shared_memory[20].s3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        /* *************************************** */


        /*   ****       Best Time Circuit     ****     */
        if (tour_complet < shared_memory[20].best_Circuit) {
            shared_memory[20].best_Circuit = tour_complet;
        }
        /*   ****       Best Time Circuit     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
             shared_memory[i].best_Circuit = tour_complet;
        }
        usleep(80);
        /* *************************************** */
    }


    return 0;
}



unsigned int generateNumber(void)
{
    return rand() * clock()%(MAX-MIN+1)+MIN;
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
    shared_memory[i].isFinished = false;
}


void initBest(void) {
    shared_memory[20].s1 = MAX;
    shared_memory[20].s2 = MAX;
    shared_memory[20].s3 = MAX;
    shared_memory[20].best_Circuit = 3*MAX;
}
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
    return rand() * clock() % (TEMPS_MAX_STAND - TEMPS_MIN_STAND + 1) + TEMPS_MIN_STAND;
}

void goOut(int i) {
    if(shared_memory[i].compteurStand > 19) {
        shared_memory[i].isOut = true;
    }
}

bool course_actif = true;
void afficherTableau(int tempsMaxCircuit , char **argv) {

    while(true){

        system("clear");

        memcpy( copyTableau, shared_memory, sizeof(copyTableau) );

        qsort(copyTableau, NUMBER_OF_CARS, sizeof(voiture), (__compar_fn_t) compare);
        sortLap();

        printf("\n\tMeilleurs temps par tour complet\n");
        printf(" =============================================================================================\n");
        printf(" |     ID   |      s1     |      s2     |      s3     |     Tour    |     LAP     |   Stand  |\n");
        printf(" |===========================================================================================|\n");

        for (int i = 0; i < NUMBER_OF_CARS; i++){
            printf(" |     %2d   |    %5d    |    %5d    |    %5d    |    %6d    |    %5d    |    %2d    | %5d\n", \
                    copyTableau[i].id, \
                    copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                    copyTableau[i].best_Circuit,\
                    copyTableau[i].lap, \
                    copyTableau[i].compteurStand, copyTableau[i].tempsTotal);
        }
        printf(" =============================================================================================\n\n");
        
        printf("bs1: %d, bs2: %d, bs3: %d et b_circuit %d\n", copyTableau[20].s1, copyTableau[20].s2, copyTableau[20].s3, copyTableau[20].best_Circuit);
         
        //si toutes les voitures on terminer la course

        if(finished(tempsMaxCircuit , argv)){
            savedFile(argv);
            break;
        }
        sleep(1);
        
    }

    course_actif = false;
    
}

int finished(int tempsMaxCircuit , char **argv) {
    for (int i = 0; i < NUMBER_OF_CARS; ++i) {
        if (shared_memory[i].tempsTotal >= tempsMaxCircuit) {
            return 1;
        }
    }
    
    return 0;
}







/**
 * Paramètrage de la session courante en fonction des arguments entrées en paramètre
 * @param argc : nombre d'arguments entrées par l'utilisateur
 * @param argv : tableau avec les arguments entrées en paramètres
 */
void define_session(int argc, char *argv[]){
    double total_km;    // Variable qui contiendra le total de kilomètres par tour

    // On vérifie si le nombre de paramètres entrées est correcte
    if (argc < 2 || argc > 4){
        perror("Invalid Parameter");
        exit(-1);
    }
    else if (!strcmp(argv[1], "RACE") && argc == 2){
        perror("Invalid Parameter");
        exit(-1);
    }
    else if (strcmp(argv[1], "RACE") && argc == 3){
        perror("Invalid Parameter");
        exit(-1);
    }

    sprintf(current_session.file_name, "%s.txt", argv[1]);  // On définit le nom du fichier à enregistrer

    // Paramètreage de la session en fonction des arguments
    if (!strcmp(argv[1], "P1") || !strcmp(argv[1], "P2")) {
        current_session.session_time = 5400;
        current_session.total_cars = 20;
    }
    else if (!strcmp(argv[1], "P3")){
    
        check_course("./data/P2.txt");
        current_session.session_time = 3600;
        current_session.total_cars = 20;
     

    }
    else if (!strcmp(argv[1], "Q1")){
        check_course("./data/P3.txt");
        current_session.session_time = 1080;
        current_session.total_cars = 20;

    }
    else if (!strcmp(argv[1], "Q2")){
        check_course("./data/Q1.txt");
        current_session.session_time = 900;
        current_session.total_cars = 15;

    }
    else if (!strcmp(argv[1], "Q3")){
        check_course("./data/Q2.txt");
        current_session.session_time = 720;
        current_session.total_cars = 10;

    }

    else if (!strcmp(argv[1], "RACE")){
        total_km = atoi(argv[2]);

        if (!total_km){
            perror("error: second argument must be an integer");
            exit(-1);
        }
        else if (total_km > 7.1 && total_km < 3.2){
            write(1, "error: length of a circuit must be between 3.2 and 7.1 km", sizeof("error: length of a circuit must be between 3.2 and 7.1 km"));
            exit(-1);
        }

        current_session.session_time = 7200;
        current_session.total_cars = 20;
    }
    else{
        write(1, "error: invalid first argument, first argument must be in [P1, P2, P3, Q1, Q2, Q3, RACE]", sizeof("error: invalid first argument, first argument must be in [P1, P2, P3, Q1, Q2, Q3, RACE]"));
        exit(-1);
    }
}

void check_course(char course[]){
    
        FILE *file;
    
        if (file = fopen(course, "r"))  //Verification de P2.txt
        {
            fclose(file);
        }
        else{
            printf("Va faire la course précédente !!");
            exit(-1);
            
        }   

}

bool savedFile(char *argv[]) {

    char fichiertxt[] = "./data/" ;

    strcat(fichiertxt  , argv[1]);
    strcat(fichiertxt , ".txt");
    
    FILE *fichier = fopen(fichiertxt, "w");
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
