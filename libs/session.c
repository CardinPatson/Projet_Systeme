#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "session.h"

#include <sys/types.h>
#include <sys/stat.h>


#define NUMBER_OF_STRING 3
#define MAX_STRING_SIZE 15


/****************************************
 * DEBUT DEFINE SESSION ET COMPAGNIE
*****************************************/

void define_session(int argc, char *argv[], unsigned int* numeroVoiture , unsigned int* qualifiedCars , Session *current_session){

    // VERIFICATION DU NOMBRE DE PARAMETRE ENTREE
    if (argc < 2 || argc > 3){
        perror("Invalid Parameter");
        exit(-1);
    }

    //NOM DU FICHIER
    sprintf(current_session->file_name, "%s.txt", argv[1]);

    //SI LE 1ER ARG == P1 || P2
    if (!strcmp(argv[1], "P1") || !strcmp(argv[1], "P2")) {
        current_session->session_time = 1000;//5400
        current_session->total_cars = 20;
        current_session->qualified = 20;
    }

        //P3
    else if (!strcmp(argv[1], "P3")){

        //CHECKER SI LA COURSE PRECEDENTE EST FAITE
        if (check_course("P2")) {
            current_session->session_time = 3600;
            current_session->total_cars = 20;
            current_session->qualified = 20;
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }
    }

        //Q1
    else if (!strcmp(argv[1], "Q1")){
        if (check_course("P3")) {
            current_session->session_time = 1080;
            current_session->total_cars = 20;
            current_session->qualified = 15;
        }
        else {
            printf("Va faire la course précédente !!");
        }
    }

        //Q2
    else if (!strcmp(argv[1], "Q2")){

        if (check_course("Q1")) {
            current_session->session_time = 900;
            current_session->total_cars = 15;
            current_session->qualified = 10;

            //RECUPERE LES 15 PREMIERS DE LA Q1 ET LES MET DANS NUMERO VOITURE
            modify_classement(numeroVoiture, "../data/Q1.txt",current_session);
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }

    }

        //Q3
    else if (!strcmp(argv[1], "Q3")){

        if (check_course("Q2")) {
            current_session->session_time = 720;
            current_session->total_cars = 10;
            current_session->qualified = 10;

            //RECUPERE LES 15 PREMIERS DE LA Q1 ET LES MET DANS NUMERO VOITURE
            modify_classement(numeroVoiture, "../data/Q2.txt" , current_session);
        }
        else {
            printf("Va faire la course précédente !!");
            exit(-1);
        }
    }
        //FINALE
    else if (!strcmp(argv[1], "FINALE")){

        current_session->session_time =  7200;
        current_session->total_cars = 20;

        //PREPARATION DE LA GRILLE DE DEPART POUR LA FINALE

        prepaClassementFinal(numeroVoiture , current_session);

    }
    else{
        perror("error: veuillez entrer un argument entre P1, P2, P3, Q1, Q2, Q3, FINALE");
        exit(-1);
    }
}

bool check_course(char course[]){

    //VERIFIE SI LE FICHIER COURSE EXITE DEJA
    char destination[20] = "../data/";
    strcat(destination, course);
    strcat(destination, ".txt");

    int fd = open(destination, O_RDONLY);

    if (fd ==-1) {
        return false;
    }

    close(fd);
    return true;
}


int modify_classement(unsigned int numberArray[], char course[], Session *current_session){
    //INSERTION DES LIGNES DU FICHIER COURSE DANS NUMBERARRAY
    FILE *myFile;
    myFile = fopen(course, "r");
    if (myFile == NULL) {
        perror("fopen() failed !");
        exit(EXIT_FAILURE);
    }
    //read file into array
    for (int i = 0; i < current_session->total_cars; i++)
    {
        fscanf(myFile, "%d", &numberArray[i]);
    }
    fclose(myFile);
    return 0;
}

/*
 * SAUVEGARDE DU CLASSEMENT DE DEPART DE LA FINALE DANS LE FICHIER CLASSEMENT.txt
 * */
void prepaClassementFinal(unsigned int qualifiedCars[] , Session *current_session ) {

    char arr[NUMBER_OF_STRING][MAX_STRING_SIZE] = {"../data/Q3.txt", "../data/Q2.txt", "../data/Q1.txt"};

    //CREATION DU FICHIER CLASSEMENT.txt
    FILE *fichier = fopen("../data/CLASSEMENT.txt", "a+");
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
    for (int i = 0; i < current_session->total_cars; i++){
        fscanf(fichier, "%d", &qualifiedCars[i]);

    }

    //FERMETURE DE CLASSEMENT.txt
    fclose(fichier);
}

/****************************************
 * FIN DEFINE SESSION ET COMPAGNIE
*****************************************/