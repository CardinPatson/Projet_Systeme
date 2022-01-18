#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>   //pour appel sys close, sleep
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>    // pour as open
#include <semaphore.h>

#define NUMBER_OF_CARS 20

//INTERVALLE DE TEMPS POUR LES SECTEURS
#define MIN 25.00
#define MAX 40.00

//INTERVALLE DE TEMPS POUR LES STANDS
#define TEMPS_MAX_STAND 25.00
#define TEMPS_MIN_STAND 12.00

//NOMBRE DE TOURS POUR LA FINALE
#define NOMBRE_TOURS_FINALE 45



typedef struct {
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
} voiture;

//MEMOIRE PARTAGEE CONTENANT TOUTES LES VOITURES
voiture *shared_memory;
voiture copyTableau[NUMBER_OF_CARS + 1];

//PARAMETRE DE LA COURSE
typedef struct{
    char file_name[20];  //FICHIER QUI VA CONTENIR LE CLASSEMENT
    unsigned int session_time;
    unsigned int total_cars;
    unsigned int qualified;
}Session;

Session current_session;


//PROTOTYPE
int faireDesTours(sem_t *semaphore, int i ,unsigned int tempsMaxCircuit );
double generateNumber(void);
void afficherTableau(sem_t *semaphore, unsigned int tempsMaxCircuit , char **argv) ;
int compare(const void * a, const void * b);
void initVoiture(int i);
void sortLap(void);
double generateStandStop(void);
bool goStand(unsigned int digit);
void goOut(int i);
int lancement(sem_t *semaphore, char **argv, const unsigned int* numeroVoiture);
int finished(unsigned int tempsMaxCircuit , int nbre_tours_finale) ;
void initBest(void) ;
void define_session(int argc, char *argv[], unsigned int* numeroVoiture);
bool savedFile(char *argv[]);
bool check_course(char course[]);
int modify_classement(unsigned int numberArray[], char course[]);
void prepaClassementFinal(void);
int finale(sem_t *semaphore, int i , unsigned int nbre_tours_max);


//TABLEAU DES VOITURES QUI SERONT QUALIFIE POUR LA FINALE
unsigned int qualifiedCars[20];


int main(int argc , char *argv[])
{
    unsigned int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};

    /***************************************************
   *           Création de la mémoire partagée        *
   ****************************************************/
    define_session(argc , argv, numeroVoiture);

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

    /******************************************************
    *                Création des sémaphores              *
    *******************************************************/
    int sem_shm_id = shmget(IPC_PRIVATE, sizeof(sem_t), 0600 | IPC_CREAT);
    if (sem_shm_id == -1) {
        perror("shmget failed !");
        exit(EXIT_FAILURE);
    }
    sem_t *semaphore = shmat(sem_shm_id, NULL, 0);
    if (semaphore == (void *) (-1)) {
        perror("shmat failed !");
        exit(EXIT_FAILURE);
    }

    sem_init(semaphore, 1, 1);



    lancement(semaphore, argv, numeroVoiture);



    /********  Détachament des segments de mémoire  *********/
    shmdt(shared_memory);

    /********  Supprimer la mémoire partagée  *********/
    shmctl(segment_id, IPC_RMID, NULL);

    /********  Destruction des sémaphores  *********/
    sem_destroy(semaphore);
    shmdt(semaphore);
    shmctl(sem_shm_id, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}


/****************************************
 * DEBUT DEFINE SESSION ET COMPAGNIE
*****************************************/

void define_session(int argc, char *argv[], unsigned int* numeroVoiture){

    // VERIFICATION DU NOMBRE DE PARAMETRE ENTREE
    if (argc < 2 || argc > 3){
        perror("Invalid Parameter");
        exit(-1);
    }

    //NOM DU FICHIER
    sprintf(current_session.file_name, "%s.txt", argv[1]);

    //SI LE 1ER ARG == P1 || P2
    if (!strcmp(argv[1], "P1") || !strcmp(argv[1], "P2")) {
        current_session.session_time = 1000; //5400
        current_session.total_cars = 20;
        current_session.qualified = 20;
    }

        //P3
    else if (!strcmp(argv[1], "P3")){

        //CHECKER SI LA COURSE PRECEDENTE EST FAITE
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

        //Q1
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

        //Q2
    else if (!strcmp(argv[1], "Q2")){
        if (check_course("Q1")) {
            current_session.session_time = 900;
            current_session.total_cars = 15;
            current_session.qualified = 10;

            //RECUPERE LES 15 PREMIERS DE LA Q1 ET LES MET DANS NUMERO VOITURE
            modify_classement(numeroVoiture, "./data/Q1.txt");
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }

    }

        //Q3
    else if (!strcmp(argv[1], "Q3")){

        if (check_course("Q2")) {
            current_session.session_time = 720;
            current_session.total_cars = 10;
            current_session.qualified = 10;

            //RECUPERE LES 15 PREMIERS DE LA Q1 ET LES MET DANS NUMERO VOITURE
            modify_classement(numeroVoiture, "./data/Q2.txt");
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }
    }
        //FINALE
    else if (!strcmp(argv[1], "FINALE")){

        current_session.session_time = 7200;
        current_session.total_cars = 20;

        //PREPARATION DE LA GRILLE DE DEPART POUR LA FINALE

        prepaClassementFinal();

    }
    else{
        perror("error: veuillez entrer un argument entre P1, P2, P3, Q1, Q2, Q3, FINALE");
        exit(-1);
    }
}

bool check_course(char course[]){

    //VERIFIE SI LE FICHIER COURSE EXITE DEJA
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


int modify_classement(unsigned int numberArray[], char course[]){

    //INSERTION DES LIGNES DU FICHIER COURSE DANS NUMBERARRAY
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
 * SAUVEGARDE DU CLASSEMENT DE DEPART DE LA FINALE DANS LE FICHIER CLASSEMENT.txt
 * */
void prepaClassementFinal(void) {

#define NUMBER_OF_STRING 3
#define MAX_STRING_SIZE 14


    char arr[NUMBER_OF_STRING][MAX_STRING_SIZE] = {"./data/Q3.txt", "./data/Q2.txt", "./data/Q1.txt"};

    //CREATION DU FICHIER CLASSEMENT.txt
    FILE *fichier = fopen("./data/CLASSEMENT.txt", "a+");
    if (fichier == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUMBER_OF_STRING; i++)
    {
        //OUVERTURE DES FICHIERS Q1 Q2 Q3
        FILE* file = fopen(arr[i], "r");
        char line[256];
        int j = 0;

        //TANT QUON A DES LIGNES DANS [Q1 Q2 Q2] ON LES METS DANS CLASSEMENT.txt
        while (fgets(line, sizeof(line), file)) {
            j++;
            strtok(line,"\n");

            if(i == 0) {
                //LES 10 PREMIERES line DE Q3 -> CLASSEMENT.txt
                if(j >= 0 && j <= 10) {
                    fprintf(fichier, "%s\n", line);
                }
            }
            if(i == 1){
                //LES 5 DERNIERES line DE Q2 -> CLASSEMENT.txt
                if(j > 10 && j <= 15) {
                    fprintf(fichier, "%s\n", line);
                }
            }
            if(i == 2){
                //LES 5 DERNIERES line DE Q1 -> CLASSEMENT.txt
                if(j > 15 && j <= 20) {
                    fprintf(fichier, "%s\n", line);
                }
            }
        }

        //FERMETURE DE Q1 Q2 Q3
        fclose(file);
    }
    //INSERTION DE CLASSEMENT.txt --> qualifiedCars
    for (int i = 0; i < 20; i++){

        fscanf(fichier, "%d", &qualifiedCars[i]);

    }

    //FERMETURE DE CLASSEMENT.txt
    fclose(fichier);
}

/****************************************
 * FIN DEFINE SESSION ET COMPAGNIE
*****************************************/



/****************************************
 * DEBUT LANCEMENT ET COMPAGNIE
*****************************************/
int lancement(sem_t *semaphore, char **argv, const unsigned int* numeroVoiture)
{
    //PERMET DE STOCKER LES MEILLEURS DANS UNE CASE SUPPLEMENTAIRE DE LA MEMOIRE PARTAGEE
    initBest();


    // CREATION DES FILS
    for (int i = 0; i < current_session.total_cars; ++i)
    {
        initVoiture(i);
        /********  échec du fork *********/
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed !");
            exit(EXIT_FAILURE);
        }

        /********  les fils *********/
        if(pid == 0) {

            //POUR LA FINALE
            if (strcmp(argv[1], "RACE") == 0){
                shared_memory[i].id = qualifiedCars[i];
                finale(semaphore, i, NOMBRE_TOURS_FINALE);
                exit(EXIT_SUCCESS);
            }

            else{
                shared_memory[i].id = numeroVoiture[i]; //attribution numéro pour chaque voiture
                faireDesTours(semaphore, i ,current_session.session_time);
                exit(EXIT_SUCCESS);
            }
        }
    }

    //AFFICHAGE PERE
    afficherTableau(semaphore, current_session.session_time , argv);
    exit(EXIT_SUCCESS);
}


/** INITIALISATION DU MEILLEUR : CELUI DANS LA CASE SUPP **/
void initBest(void) {
    shared_memory[20].s1 = MAX;
    shared_memory[20].s2 = MAX;
    shared_memory[20].s3 = MAX;
    shared_memory[20].best_Circuit = 3*MAX;
}

/** INITIALISATION DUNE VOITURE **/
void initVoiture(int i) {
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
 * DEBUT  COURSE ET COMPAGNIE
*****************************************/
int faireDesTours(sem_t *semaphore, int i , unsigned int tempsMaxCircuit ) {

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
            goOut(i);
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
        if (tour_complet < shared_memory[20].best_Circuit) {
            shared_memory[20].best_Circuit = tour_complet;
        }
        /*   ****       Best Time Circuit de la voiture actuelle     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
            shared_memory[i].best_Circuit = tour_complet;
        }
        sem_post(semaphore);

        sleep(1);
        /* *************************************** */
    }
    return 0;
}

int finale(sem_t *semaphore, int i , unsigned int nbre_tours_max){

    double tour_complet;
    srand(time(NULL) + getpid());
    while (shared_memory[i].nbre_tours <= nbre_tours_max && !shared_memory[i].isOut) //time pas dépassée
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
            goOut(i);
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
        if (tour_complet < shared_memory[20].best_Circuit) {
            shared_memory[20].best_Circuit = tour_complet;
        }

        /*   ****       Best Time Circuit de la voiture actuelle     ****     */
        if (tour_complet < shared_memory[i].best_Circuit) {
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

void goOut(int i) {

    //SI JATTEINT LA LIMITE DE PASSAGE AU STAND : 19
    if(shared_memory[i].compteurStand > 19) {
        shared_memory[i].isOut = true;
    }
}

/****************************
 * FIN COURSE ET COMPAGNIE
*****************************/

/*****************************
 * FIN LANCEMENT ET COMPAGNIE
******************************/


/******************************
 * DEBUT AFFICHAGE ET COMPAGNIE
*******************************/

void afficherTableau(sem_t *semaphore, unsigned int tempsMaxCircuit , char **argv) {

    while(true){
        system("clear");

        sem_wait(semaphore); //ouvre la porte
        memcpy( copyTableau, shared_memory, sizeof(copyTableau) );
        sem_post(semaphore); // fermer la porte

        //TRIE DU TABLEAU + DIFFERENCE DE TEMPS ENTRE VOITURE
        qsort(copyTableau, current_session.total_cars,  sizeof(voiture), compare); // (__compar_fn_t)
        sortLap();

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
        if(finished(tempsMaxCircuit , NOMBRE_TOURS_FINALE)){

            savedFile(argv);
            break;
        }
        sleep(1);
    }
}


void sortLap() {
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
    const voiture *voitureA = (voiture *)a;
    const voiture *voitureB = (voiture *)b;
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

int finished(unsigned int tempsMaxCircuit , int nbre_tours_finale) {
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
/******************************
 * FIN AFFICHAGE ET COMPAGNIE
*******************************/
