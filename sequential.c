#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define NUMBER_OF_CARS 20
#define MIN 25000 // time generator
#define MAX 40000

#define TEMPS_MAX_STAND 5000
#define TEMPS_MIN_STAND 2000


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

    unsigned int lap;

    unsigned int compteurStand;
} voiture;

voiture tabStuctVoiture[NUMBER_OF_CARS];
voiture copyTableau[NUMBER_OF_CARS];



int faireDesTours(int i);
unsigned int generateNumber(void);
void afficherTableau(void);
int compare (const void * a, const void * b);
void initVoiture(int i);
void sortLap(void);
unsigned int generateStandStop(void);

unsigned int recupLastDigit(unsigned int digit);
bool allerStand(unsigned int digit);



int main(void)
{
    srand( time(NULL) );

    for (int i = 0; i < NUMBER_OF_CARS; i++)
    {
        tabStuctVoiture[i].id = numeroVoiture[i]; // attribution numéro
        faireDesTours(i); //5400000
    }

    // copy of array
    memcpy( copyTableau, tabStuctVoiture, sizeof(tabStuctVoiture) );

    //trier Tableau;
    qsort( copyTableau, NUMBER_OF_CARS, sizeof(voiture), compare );
    sortLap();

    afficherTableau();

    return 0;
}



unsigned int tempsMaxCircuit = 5400000;

int faireDesTours( int i ) {

    initVoiture(i);


    unsigned int tour_complet;


    while ( tabStuctVoiture[i].tempsTotal <= tempsMaxCircuit ) //time pas dépassée
    {
        tour_complet = 0;

        /*   ****       S1     ****     */
        tabStuctVoiture[i].s1 = generateNumber();
        if (tabStuctVoiture[i].s1 < tabStuctVoiture[i].best_S1) {
            tabStuctVoiture[i].best_S1 = tabStuctVoiture[i].s1;
        }
        tabStuctVoiture[i].tempsTotal += tabStuctVoiture[i].s1;
        tour_complet += tabStuctVoiture[i].s1;
        /* *************************************** */

        if (tabStuctVoiture[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }
        /*   ****       S2     ****     */
        tabStuctVoiture[i].s2 = generateNumber();
        if (tabStuctVoiture[i].s2 < tabStuctVoiture[i].best_S2) {
            tabStuctVoiture[i].best_S2 = tabStuctVoiture[i].s2;
        }
        tabStuctVoiture[i].tempsTotal += tabStuctVoiture[i].s2;
        tour_complet += tabStuctVoiture[i].s2;
        /* *************************************** */

        if (tabStuctVoiture[i].tempsTotal >= tempsMaxCircuit)
        {
            break;
        }

        /*   ****       aller au Stand     ****     */

        // si il va rentre au stand
        unsigned int timeSupplementaire = 0;

        //si dernier digit ==9 ==> go stand secteur3 + generer le temps sup
        if (allerStand(tabStuctVoiture[i].s2)) {
            tabStuctVoiture[i].compteurStand += 1;
            timeSupplementaire = generateStandStop();
        }
        /* *************************************** */



        /*   ****       S3     ****     */
        tabStuctVoiture[i].s3 = generateNumber();
        tabStuctVoiture[i].s3 += timeSupplementaire;
        if (tabStuctVoiture[i].s3 < tabStuctVoiture[i].best_S3) {
            tabStuctVoiture[i].best_S3 = tabStuctVoiture[i].s3;
        }
        tabStuctVoiture[i].tempsTotal += tabStuctVoiture[i].s3;
        tour_complet += tabStuctVoiture[i].s3;
        /* *************************************** */


        /*   ****       Best Time Circuit     ****     */
        if (tour_complet < tabStuctVoiture[i].best_Circuit) {
            tabStuctVoiture[i].best_Circuit = tour_complet;
        }
        /* *************************************** */

        if (tabStuctVoiture[i].tempsTotal >= tempsMaxCircuit)
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
    printf(" =============================================================================================\n");
    printf(" |     ID   |      s1     |      s2     |      s3     |     Tour    |     LAP     |   Stand  |\n");
    printf(" |===========================================================================================|\n");
    for (int i = 0; i < NUMBER_OF_CARS; i++){
        printf(" |     %2d   |    %5d    |    %5d    |    %5d    |    %6d    |    %4d    |    %2d    |\n", \
                copyTableau[i].id, \
                copyTableau[i].s1, copyTableau[i].s2, copyTableau[i].s3, \
                copyTableau[i].best_Circuit,\
                copyTableau[i].lap, \
                copyTableau[i].compteurStand);
    }
    printf(" =============================================================================================\n\n");
}



int compare(const void * a, const void * b)
{
    voiture *voitureA = (voiture *)a;
    voiture *voitureB = (voiture *)b;

    return ( voitureA->best_Circuit - voitureB->best_Circuit );
}


void initVoiture(int i) {
    tabStuctVoiture[i].s1 = 0;
    tabStuctVoiture[i].s2 = 0;
    tabStuctVoiture[i].s3 = 0;

    tabStuctVoiture[i].best_S1 = MAX;
    tabStuctVoiture[i].best_S2 = MAX;
    tabStuctVoiture[i].best_S3 = MAX;

    tabStuctVoiture[i].best_Circuit = 3 * MAX;
    tabStuctVoiture[i].tempsTotal = 0;
}



void sortLap(void) {

    unsigned int difference;

    for (int i = 1; i < NUMBER_OF_CARS; i++)
    {
        copyTableau[i].lap = 0;

        difference = ( copyTableau[i].best_Circuit - copyTableau[i -1].best_Circuit );

        copyTableau[i].lap = difference;
    }
}

unsigned int recupLastDigit(unsigned int digit) {
    return (digit % 10);
}

bool allerStand(unsigned int digit) {

    //si 9 il va au stand
    if(recupLastDigit(digit) == 9) {
        return true;
    }
    else {
        return false;
    }
}

unsigned int generateStandStop(void){
    return rand()%(TEMPS_MAX_STAND - TEMPS_MIN_STAND + 1)+ TEMPS_MIN_STAND;
}
