#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/random.h>

#define NUMBER_OF_CARS 20

int faireDesTours(int i, int tempsCircuit);
int generateNumber(int min, int max);
int array_min(int *array, int size);
double convertir_en_seconds(int kiloSecond);
int addMimTime(int numVoiture, double time_min);


int numeroVoiture[NUMBER_OF_CARS] = {44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63};


typedef struct {

    unsigned int id;

    unsigned int *temps_S1;
    unsigned int *temps_S2;
    unsigned int *temps_S3;
    unsigned int *temps_circuit;

    unsigned int cmp_S1;
    unsigned int cmp_S2;
    unsigned int cmp_S3;
    unsigned int cmp_temps_circuit;

    unsigned int S1_min;
    unsigned int S2_min;
    unsigned int S3_min;
    unsigned int temps_circuit_min;

    unsigned int tempsTotal;
} voiture;




voiture tabStuctVoiture[NUMBER_OF_CARS];


int bestTime_S1[20][2];
int bestTime_S2[20][2];
int bestTime_S3[20][2];

int bestTime_circuit[20][2];



int main(void)
{
    srand( time(NULL) );


    for (int i = 0; i < NUMBER_OF_CARS; i++)
    {
        tabStuctVoiture[i].id = numeroVoiture[i]; //chaque voiture à un numéro
        faireDesTours(i, 5400000);

        tabStuctVoiture[i].temps_circuit_min = array_min( tabStuctVoiture[i].temps_circuit, tabStuctVoiture[i].cmp_temps_circuit );
        bestTime_circuit[i][0] = tabStuctVoiture[i].id;
        bestTime_circuit[i][1] = tabStuctVoiture[i].temps_circuit_min;
    }

    int temp = 0;

    //Sort the array in ascending order    
    for (int i = 0; i < 20; i++) {     
        for (int j = i+1; j < 20; j++) {     
           if(bestTime_circuit[i][1] > bestTime_circuit[j][1]) {    
               temp = bestTime_circuit[i][1];    
               bestTime_circuit[i][1] = bestTime_circuit[j][1];    
               bestTime_circuit[j][1] = temp;    
           }     
        }     
    }    
        
    printf("\n");    
        
    //Displaying elements of array after sorting    
    printf("Best time in circuit for each car ! ascending order: \n");
    for (int i = 0; i < 20; i++) {     
        printf("Voiture: %d ==> min: %d\n\n", bestTime_circuit[i][0], bestTime_circuit[i][1]);
    }

    printf("**********************************\n");




    return 0;
}

int faireDesTours(int i, int tempsCircuit) {

    int cmpAlloc = 0;
    int numero = 0;


    if( (cmpAlloc == 0) && (tabStuctVoiture[i].tempsTotal <= tempsCircuit) )
    {
        /*   ****       S1     ****     */
        numero = generateNumber(25000, 40000);
        tabStuctVoiture[i].temps_S1 = calloc(1, sizeof(int));
        if (tabStuctVoiture[i].temps_S1 == NULL) {
            perror("Problem calloc() !");
            exit(EXIT_FAILURE);
        }
        tabStuctVoiture[i].temps_S1[0] = numero;
        tabStuctVoiture[i].tempsTotal += numero;
        /* *************************************** */


        /*   ****       S2     ****     */
        numero = generateNumber(25000, 40000);
        tabStuctVoiture[i].temps_S2 = calloc(1, sizeof(int));
        if (tabStuctVoiture[i].temps_S2 == NULL) {
            perror("Problem calloc() !");
            exit(EXIT_FAILURE);
        }
        tabStuctVoiture[i].temps_S2[0] = numero;
        tabStuctVoiture[i].tempsTotal += numero;
        /* *************************************** */


        /*   ****       S3     ****     */
        numero = generateNumber(25000, 40000);
        tabStuctVoiture[i].temps_S3 = calloc(1, sizeof(int));
        if (tabStuctVoiture[i].temps_S3 == NULL) {
            perror("Problem calloc() !");
            exit(EXIT_FAILURE);
        }
        tabStuctVoiture[i].temps_S3[0] = numero;
        tabStuctVoiture[i].tempsTotal += numero;
        /* *************************************** */


        /*   ****       Temps circuit     ****     */
        tabStuctVoiture[i].temps_circuit = calloc(1, sizeof(int));
        if (tabStuctVoiture[i].temps_circuit == NULL) {
            perror("Problem calloc() !");
            exit(EXIT_FAILURE);
        }
        tabStuctVoiture[i].temps_circuit[0] = tabStuctVoiture[i].tempsTotal;
        /* *************************************** */


        cmpAlloc++;
    }

    if ((cmpAlloc > 0) && (tabStuctVoiture[i].tempsTotal <= tempsCircuit)) 
    {

        tabStuctVoiture[i].cmp_S1 = 1;
        tabStuctVoiture[i].cmp_S2 = 1;
        tabStuctVoiture[i].cmp_S3 = 1;
        tabStuctVoiture[i].cmp_temps_circuit = 1;

        int cmpTempsSecteur;


        while ((cmpAlloc > 0) && (tabStuctVoiture[i].tempsTotal <= tempsCircuit))
        {
            cmpTempsSecteur = 0;
            
            if (tabStuctVoiture[i].tempsTotal >= tempsCircuit)
            {
                break;
            }
            /*   ****       S1     ****     */
            numero = generateNumber(25000, 40000);
            tabStuctVoiture[i].temps_S1 = realloc(tabStuctVoiture[i].temps_S1, (cmpAlloc + 1) * sizeof(int));
            if (tabStuctVoiture[i].temps_S1 == NULL) {
                perror("Problem realloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[i].cmp_S1++;
            tabStuctVoiture[i].temps_S1[cmpAlloc] = numero;
            cmpTempsSecteur += numero;
            tabStuctVoiture[i].tempsTotal += numero;
            /* *************************************** */


            if (tabStuctVoiture[i].tempsTotal >= tempsCircuit)
            {
                break;
            }
            /*   ****       S2     ****     */
            numero = generateNumber(25000, 40000);
            tabStuctVoiture[i].temps_S2 = realloc(tabStuctVoiture[i].temps_S2, (cmpAlloc + 1) * sizeof(int));
            if (tabStuctVoiture[i].temps_S2 == NULL) {
                perror("Problem realloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[i].cmp_S2++;
            tabStuctVoiture[i].temps_S2[cmpAlloc] = numero;
            cmpTempsSecteur += numero;
            tabStuctVoiture[i].tempsTotal += numero;
            /* *************************************** */


            if (tabStuctVoiture[i].tempsTotal >= tempsCircuit)
            {
                break;
            }
            /*   ****       S3     ****     */
            numero = generateNumber(25000, 40000);
            tabStuctVoiture[i].temps_S3 = realloc(tabStuctVoiture[i].temps_S3, (cmpAlloc + 1) * sizeof(int));
            if (tabStuctVoiture[i].temps_S3 == NULL) {
                perror("Problem realloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[i].cmp_S3++;
            tabStuctVoiture[i].temps_S3[cmpAlloc] = numero;
            cmpTempsSecteur += numero;
            tabStuctVoiture[i].tempsTotal += numero;
            /* *************************************** */


            /*   ****       Temps circuit     ****     */
            tabStuctVoiture[i].temps_circuit = realloc(tabStuctVoiture[i].temps_circuit, (cmpAlloc + 1) * sizeof(int));
            if (tabStuctVoiture[i].temps_circuit == NULL) {
                perror("Problem realloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[i].temps_circuit[cmpAlloc] = cmpTempsSecteur;
            tabStuctVoiture[i].cmp_temps_circuit++;
            /* *************************************** */


            cmpAlloc++;
        }

    }


    return 0;
}


int generateNumber(int min, int max) 
{
    int range = (max - min); 
    int div = RAND_MAX / range;
    return min + (rand() / div);
}



int array_min(int *array, int size)
{
    int min = array[0];
    int i;
    for (i = 1; i < size; i++)
        if (min > array[i])
            min = array[i];

    return min;
}


double convertir_en_seconds(int kiloSecond) {
    return kiloSecond / 1000.0;
}


int addMimTime(int numVoiture, double time_min) {

    // sert à rien pour l'instant !!

    return 0;
}


/*

for (int i = 0; i < NUMBER_OF_CARS; i++)
    {
        faireDesTours(i, 5400000);

        printf("Voiture n°%d --> Temps Total : %d\n", tabStuctVoiture[i].id, tabStuctVoiture[i].tempsTotal);

        tabStuctVoiture[i].S1_min = array_min( tabStuctVoiture[i].temps_S1, tabStuctVoiture[i].cmp_S1 );
        bestTime_S1[i][0] = tabStuctVoiture[i].id;
        bestTime_S1[i][1] = tabStuctVoiture[i].S1_min;

        tabStuctVoiture[i].S2_min = array_min( tabStuctVoiture[i].temps_S2, tabStuctVoiture[i].cmp_S2 );
        bestTime_S2[i][0] = tabStuctVoiture[i].id;
        bestTime_S2[i][1] = tabStuctVoiture[i].S2_min;

        tabStuctVoiture[i].S3_min = array_min( tabStuctVoiture[i].temps_S3, tabStuctVoiture[i].cmp_S3 );
        bestTime_S3[i][0] = tabStuctVoiture[i].id;
        bestTime_S3[i][1] = tabStuctVoiture[i].S3_min;
    }
        printf("------------------------------------\n\n\n");

        printf("Best time in S1 for each car:\n\n");
        for(int x=0; x < NUMBER_OF_CARS; x++) {

            printf("Voiture: %d ==> min: %d\n\n", bestTime_S1[x][0], bestTime_S1[x][1]);

        }
        printf("**********************************\n");

        printf("Best time in S2 for each car:\n\n");
        for(int x=0; x < NUMBER_OF_CARS; x++) {

            printf("Voiture: %d ==> min: %d\n\n", bestTime_S2[x][0], bestTime_S2[x][1]);

        }
        printf("**********************************\n");


        printf("Best time in S3 for each car:\n\n");
        for(int x=0; x < NUMBER_OF_CARS; x++) {

            printf("Voiture: %d ==> min: %d\n\n", bestTime_S3[x][0], bestTime_S3[x][1]);

        }
        printf("**********************************\n");



                for (int j = 0; j < tabStuctVoiture[0].cmp_temps_circuit; j++)
        {
            printf("%d\n", tabStuctVoiture[0].temps_circuit[j]);
        }

*/
