#ifndef __SESSION_H_
#define __SESSION_H_

//PARAMETRE DE LA COURSE
typedef struct{
    char file_name[20];  //FICHIER QUI VA CONTENIR LE CLASSEMENT
    unsigned int session_time;
    unsigned int total_cars;
    unsigned int qualified;
}Session;

void define_session(int argc, char *argv[], unsigned int* numeroVoiture , unsigned int* qualifiedCars , Session *current_session);
bool check_course(char course[]);
int modify_classement(unsigned int numberArray[], char course[], Session *current_session);
void prepaClassementFinal(unsigned int qualifiedCars[] , Session *current_session );


#endif