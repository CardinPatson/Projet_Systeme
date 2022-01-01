#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUMBER_OF_CARS 20
#define MIN 25 // time generator
#define MAX 40
#define TEMPS_MAX_STAND 25
#define TEMPS_MIN_STAND 12
#define NOMBRE_TOURS_FINALE 45


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
    unsigned int nbre_tours;
    bool isFinished ;
    
} voiture;

voiture *shared_memory;
voiture copyTableau[NUMBER_OF_CARS + 1];

typedef struct{
    char file_name[20];
    unsigned int session_time;
    unsigned int total_cars;
    unsigned int qualified;
}Session;

Session current_session;


int faireDesTours( int i , unsigned int tempsMaxCircuit );
unsigned int generateNumber(void);
void afficherTableau(unsigned int tempsMaxCircuit , char **argv);
unsigned int compare (const void * a, const void * b);
void initVoiture(int i);
void sortLap(void);
unsigned int generateStandStop(void);
bool goStand(unsigned int digit);
void goOut(int i);
int lancement(char **argv, const unsigned int* numeroVoiture);
int finished(unsigned int tempsMaxCircuit , int nbre_tours_finale) ;
void initBest(void) ;
void define_session(int argc, char *argv[], unsigned int* numeroVoiture);
bool savedFile(char *argv[]);
bool check_course(char course[]);
int modify(unsigned int numberArray[], char course[]);
void prepa_qualifiedCars(void);
void prepaClassementFinal(void);
void removeNewLineChar(char *ptr);
int finale(int i , unsigned int nbre_tours_max);


unsigned int qualifiedCars[20];

int main(int argc , char *argv[])
{
    unsigned int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};
    /***************************************************
   *           Création de la mémoire partagée        *
   ****************************************************/
    int segment_id = shmget(IPC_PRIVATE, sizeof(voiture) * current_session.total_cars+1, 0666 | IPC_CREAT);
    if (segment_id == -1) {
        perror("shmget() failed !");
        exit(EXIT_FAILURE);
    }
    shared_memory = shmat(segment_id, NULL, 0);
    if (shared_memory == (void *) (-1)) {
        perror("shmat() failed !");
        exit(EXIT_FAILURE);
    }
    define_session(argc , argv, numeroVoiture);
    lancement(argv, numeroVoiture);
    /********  detach & Supprimer la mémoire partagée  *********/
    shmdt(shared_memory);
    shmctl(segment_id, IPC_RMID, NULL);
    return 0;
}


int lancement(char **argv, const unsigned int* numeroVoiture)
{   initBest();
    /**********************************************************
    *               Création des fils/voitures               *
    **********************************************************/

    for (int i = 0; i < current_session.total_cars; ++i)
    {
        initVoiture(i);
        /********  échec du fork *********/
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed !");
            exit(EXIT_FAILURE);
        }
        /********  le fils *********/
        if(pid == 0) {
            if (strcmp(argv[1], "RACE") == 0){
                // printf("je suis dans le Race");
                shared_memory[i].id = qualifiedCars[i];
                finale(i, NOMBRE_TOURS_FINALE);
                exit(EXIT_SUCCESS);
            }
            else{
                // printf("je ne ")
                shared_memory[i].id = numeroVoiture[i]; //attribution numéro pour chaque voiture
                faireDesTours(i ,  current_session.session_time);
                exit(EXIT_SUCCESS);
        }
        }
    }
    afficherTableau(current_session.session_time , argv);
    exit(EXIT_SUCCESS);
}


int faireDesTours( int i , unsigned int tempsMaxCircuit ) {
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
        /*   ****       Best Time Circuit de toutes les voitures     ****     */
        if (tour_complet < shared_memory[20].best_Circuit) {
            shared_memory[20].best_Circuit = tour_complet;
        }
        /*   ****       Best Time Circuit de la voiture actuelle     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
            shared_memory[i].best_Circuit = tour_complet;
        }
        usleep(80);
        /* *************************************** */
    }
    return 0;
}

int finale(int i , unsigned int nbre_tours_max){
    
    unsigned int tour_complet;
    srand(time(NULL) + getpid());
    while (shared_memory[i].nbre_tours <= nbre_tours_max && !shared_memory[i].isOut) //time pas dépassée
    {
        tour_complet = 0;
        /*   ****       S1     ****     */
        shared_memory[i].s1 = generateNumber();
        if (shared_memory[i].s1 < shared_memory[20].s1) {
            shared_memory[20].s1 = shared_memory[i].s1;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s1;
        tour_complet += shared_memory[i].s1;
        

        /*   ****       S2     ****     */
        shared_memory[i].s2 = generateNumber();
        if (shared_memory[i].s2 < shared_memory[20].s2) {
            shared_memory[20].s2 = shared_memory[i].s2;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s2;
        tour_complet += shared_memory[i].s2;


        /**STAND*/
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

        /*   ****       S3     ****     */
        if (shared_memory[i].s3 < shared_memory[20].s3) {
            shared_memory[20].s3 = shared_memory[i].s3;
        }
        shared_memory[i].tempsTotal += shared_memory[i].s3;
        tour_complet += shared_memory[i].s3;
        
        /* *************************************** */

        /*   ****       Best Time Circuit de toutes les voitures     ****     */
        if (tour_complet < shared_memory[20].best_Circuit) {
            shared_memory[20].best_Circuit = tour_complet;
        }

        /*   ****       Best Time Circuit de la voiture actuelle     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
            shared_memory[i].best_Circuit = tour_complet;
        }
        usleep(80);
        /* *************************************** */
        shared_memory[i].nbre_tours++;
    }

    return 0 ;
}

unsigned int generateNumber(void) {
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
    shared_memory[i].nbre_tours = 0;
}

void initBest(void) {
    shared_memory[20].s1 = MAX;
    shared_memory[20].s2 = MAX;
    shared_memory[20].s3 = MAX;
    shared_memory[20].best_Circuit = 3*MAX;
}

void sortLap() {
    unsigned int difference;
    for (int i = 1; i < current_session.total_cars; i++)
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

void afficherTableau(unsigned int tempsMaxCircuit , char **argv) {
    while(true){
        system("clear");
        memcpy( copyTableau, shared_memory, sizeof(copyTableau) );
        qsort(copyTableau, current_session.total_cars, sizeof(voiture), (__compar_fn_t) compare);
        sortLap();
        printf("\n\tMeilleurs temps par tour complet\n");
        printf(" =============================================================================================\n");
        printf(" |     ID   |      s1     |      s2     |      s3     |     Tour    |     LAP     |   Stand  |\n");
        printf(" |===========================================================================================|\n");
        for (int i = 0; i < current_session.total_cars; i++){
            printf(" |     %2d   |    %5d    |    %5d    |    %5d    |    %5d    |    %5d    |    %2d    | %5d    | %2d  \n", \
                    copyTableau[i].id, \
                    copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                    copyTableau[i].best_Circuit,\
                    copyTableau[i].lap, \
                    copyTableau[i].compteurStand, copyTableau[i].tempsTotal, \
                    copyTableau[i].nbre_tours);
        }
        printf(" =============================================================================================\n\n");
        printf("bs1: %d, bs2: %d, bs3: %d et b_circuit %d\n", copyTableau[20].s1, copyTableau[20].s2, copyTableau[20].s3, copyTableau[20].best_Circuit);
        //si toutes les voitures on terminer la course
        if(finished(tempsMaxCircuit , NOMBRE_TOURS_FINALE)){
            savedFile(argv);
            break;
        }
        sleep(1);
    }
}

int finished(unsigned int tempsMaxCircuit , int nbre_tours_finale) {
    for (int i = 0; i < current_session.total_cars; ++i) {
        if (shared_memory[i].tempsTotal >= tempsMaxCircuit) {
            return 1;
        }
        else if (shared_memory[i].nbre_tours >= nbre_tours_finale) {
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
void define_session(int argc, char *argv[], unsigned int* numeroVoiture){
    double total_km = 7.004;  // Variable qui contiendra le nombre de km/tour
    // On vérifie si le nombre de paramètres entrées est correcte
    if (argc < 2 || argc > 3){
        perror("Invalid Parameter");
        exit(-1);
    }
    // else if (!strcmp(argv[1], "RACE") && argc == 2){
    //     perror("Invalid Parameter");
    //     exit(-1);
    // }
    // else if (strcmp(argv[1], "RACE") != 0 && argc == 3){
    //     perror("Invalid Parameter");
    //     exit(-1);
    // }
    sprintf(current_session.file_name, "%s.txt", argv[1]);  // On définit le nom du fichier à enregistrer
    // Paramètreage de la session en fonction des arguments
    // == 0 -> exactement meme que !
    if (!strcmp(argv[1], "P1") || !strcmp(argv[1], "P2")) {
        current_session.session_time = 1000; //5400
        current_session.total_cars = 20;
        current_session.qualified = 20;
    }
    else if (!strcmp(argv[1], "P3")){
        if (check_course("P2")) {
            current_session.session_time = 1000; //3600
            current_session.total_cars = 20;
            current_session.qualified = 20;
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }
    }

    else if (!strcmp(argv[1], "Q1")){
        if (check_course("P3")) {
            current_session.session_time = 1080;
            current_session.total_cars = 20;
            current_session.qualified = 15;
        }
        else {
            printf("Va faire la course précédente !!");
        }
    }

    else if (!strcmp(argv[1], "Q2")){
        if (check_course("Q1")) {
            current_session.session_time = 900;
            current_session.total_cars = 15;
            current_session.qualified = 10;
            modify(numeroVoiture, "./data/Q1.txt");
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }

    }
    else if (!strcmp(argv[1], "Q3")){
        //ICI VA MODIFIER !
        if (check_course("Q2")) {
            current_session.session_time = 720;
            current_session.total_cars = 10;
            current_session.qualified = 10;
            modify(numeroVoiture, "./data/Q2.txt");
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }
    }
    else if (!strcmp(argv[1], "RACE")){
        // add total_km to cuurent_session
        current_session.session_time = 7200;
        current_session.total_cars = 20;
        prepa_qualifiedCars();
        prepaClassementFinal();


        /**Test pour le tableau qualified cars*/
        for (int i = 0; i < 20; ++i) {
            printf("%d\n", qualifiedCars[i]);
        }
       
    }
    else{
        perror("error: invalid first argument, first argument must be in [P1, P2, P3, Q1, Q2, Q3, RACE]");
        exit(-1);
    }
}

bool check_course(char course[]){
    char destination[20] = "./data/";
    strcat(destination, course);
    strcat(destination, ".txt");

    int fd = open(destination, O_RDONLY);

    if (fd ==-1) {
        return false;
    }

    close(fd);
    return true;
}

bool savedFile(char *argv[]) {
    char fichiertxt[20] = "./data/";
    strcat(fichiertxt, argv[1]);
    strcat(fichiertxt, ".txt");

    FILE *fichier = fopen(fichiertxt, "w");
    if (fichier == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < current_session.total_cars; ++i) {
        fprintf(fichier, "%d\n", copyTableau[i].id);
    }
    fclose(fichier);
    return true;
}


void prepa_qualifiedCars(void) {

    FILE *myFile;
    myFile = fopen("./data/Q3.txt", "r");
    if (myFile == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }
    //read file into array
    for (int i = 0; i < 10; i++)
    {
        fscanf(myFile, "%d", &qualifiedCars[i]);
    }
    fclose(myFile);

    myFile = fopen("./data/Q2.txt", "r");
    if (myFile == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }
    int temp[20];
    //read file into array
    for (int i = 0; i < 15; i++)
    {
        if (i >= 10) {
            fscanf(myFile, "%d", &qualifiedCars[i]);
        }
        else{
            fscanf(myFile, "%d", &temp[i]);
        }
    }
    fclose(myFile);

    // 15 --> 20
    myFile = fopen("./data/Q1.txt", "r");
    if (myFile == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }
    //read file into array
    for (int i = 0; i < 20; i++)
    {
        if (i >= 15) {
            fscanf(myFile, "%d", &qualifiedCars[i]);
        }
        else{

            fscanf(myFile, "%d", &temp[i]);
        }
    }
    fclose(myFile);
}

int modify(unsigned int numberArray[], char course[]){

    FILE *myFile;
    myFile = fopen(course, "r");
    if (myFile == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }
    //read file into array
    for (int i = 0; i < current_session.total_cars; i++)
    {
        fscanf(myFile, "%d", &numberArray[i]);
    }
    fclose(myFile);
    return 0;
}

/*
 * préparer la grille de départ
 * */
void prepaClassementFinal(void) {

#define NUMBER_OF_STRING 3
#define MAX_STRING_SIZE 14

    
    char arr[NUMBER_OF_STRING][MAX_STRING_SIZE] = {"./data/Q3.txt", "./data/Q2.txt", "./data/Q1.txt"};

    FILE *fichier = fopen("./data/CLASSEMENT.txt", "a+");
    if (fichier == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUMBER_OF_STRING; i++)
    {
        FILE* file = fopen(arr[i], "r");
        char line[256];
        int j = 0;

        while (fgets(line, sizeof(line), file)) {
            j++;
            strtok(line,"\n");

            if(i == 0) {
                //Recuperer les 10 premieres lignes dans le fichier Q3 et mettre dans line 
                if(j >= 0 && j <= 10) {
                    fprintf(fichier, "%s\n", line);
                }
            }
            if(i == 1){
                //Recuperer les lignes 11 à 15 de Q2
                if(j > 10 && j <= 15) {
                    fprintf(fichier, "%s\n", line);
                }
            }
            if(i == 2){
                //Récuperer les ligne 16 à 20 de Q3
                if(j > 15 && j <= 20) {
                    fprintf(fichier, "%s\n", line);
                }
            }
        }
        fclose(file);
    }
    for(int i = 0 ; i<20 ; i++ ){
        fscanf(fichier, "%d" , qualifiedCars[i]);
     
    }
    fclose(fichier);
}

void removeNewLineChar(char *ptr)
{
    while((ptr != NULL) && (*ptr != '\n'))
    {
        ++ptr;
    }
    *ptr = '\0';
}
