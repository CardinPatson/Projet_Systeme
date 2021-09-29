#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#define NUMBER_OF_CARS 20

int faireDesTours(int i);
int generateNumber(int min, int max);


typedef struct {

        int *temps_S1;
        int *temps_S2;
        int *temps_S3;

        int tempsTotal;
} voiture;

voiture tabStuctVoiture[NUMBER_OF_CARS];



int main(void)
{
    srand( time(NULL) );

    for (int i = 0; i < NUMBER_OF_CARS; i++) {

        pid_t pid = fork();

        if (pid < 0) {
            perror("Problème avec le Fork() !");
            exit(1);
        }

        else if (pid == 0) {
            faireDesTours(i);
            printf("%d\n\n", tabStuctVoiture[i].tempsTotal);
            exit(0);
        }
    }


    return 0;
}


int faireDesTours(int i) {

    srand( time(NULL) );

    voiture F1_Cars;

    int cmpAlloc = 0;
    int numero = 0;
        if( (cmpAlloc == 0) && (F1_Cars.tempsTotal <= 5400) )
        {
            /*   ****       S1     ****     */
            numero = generateNumber(25, 40);
            F1_Cars.temps_S1 = calloc(1, sizeof(int));
            if (F1_Cars.temps_S1 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            F1_Cars.temps_S1[0] = numero;
            F1_Cars.tempsTotal += numero;
            /* *************************************** */


            /*   ****       S2     ****     */
            numero = generateNumber(25, 40);
            F1_Cars.temps_S2 = calloc(1, sizeof(int));
            if (F1_Cars.temps_S2 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            F1_Cars.temps_S2[0] = numero;
            F1_Cars.tempsTotal += numero;
            /* *************************************** */


            /*   ****       S3     ****     */
            numero = generateNumber(25, 40);
            F1_Cars.temps_S3 = calloc(1, sizeof(int));
            if (F1_Cars.temps_S3 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            F1_Cars.temps_S3[0] = numero;
            F1_Cars.tempsTotal += numero;
            /* *************************************** */


            cmpAlloc++;
        }

        if ((cmpAlloc > 0) && (F1_Cars.tempsTotal <= 5400)) {

            while ((cmpAlloc > 0) && (F1_Cars.tempsTotal <= 5400))
            {
                if (F1_Cars.tempsTotal >= 5400)
                {
                    break;
                }
                /*   ****       S1     ****     */
                numero = generateNumber(25, 40);
                F1_Cars.temps_S1 = realloc(F1_Cars.temps_S1, (cmpAlloc + 1) * sizeof(int));
                if (F1_Cars.temps_S1 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                
                F1_Cars.temps_S1[cmpAlloc] = numero;
                F1_Cars.tempsTotal += numero;
                /* *************************************** */


                if (F1_Cars.tempsTotal >= 5400)
                {
                    break;
                }
                /*   ****       S2     ****     */
                numero = generateNumber(25, 40);
                F1_Cars.temps_S2 = realloc(F1_Cars.temps_S2, (cmpAlloc + 1) * sizeof(int));
                if (F1_Cars.temps_S2 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                
                F1_Cars.temps_S2[cmpAlloc] = numero;
                F1_Cars.tempsTotal += numero;
                /* *************************************** */


                if (F1_Cars.tempsTotal >= 5400)
                {
                    break;
                }
                /*   ****       S3     ****     */
                numero = generateNumber(25, 40);
                F1_Cars.temps_S3 = realloc(F1_Cars.temps_S3, (cmpAlloc + 1) * sizeof(int));
                if (F1_Cars.temps_S3 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                
                F1_Cars.temps_S3[cmpAlloc] = numero;
                F1_Cars.tempsTotal += numero;
                /* *************************************** */


                cmpAlloc++;
            }
        }

        tabStuctVoiture[i] = F1_Cars;

        free(F1_Cars.temps_S1);
        free(F1_Cars.temps_S2);
        free(F1_Cars.temps_S3);

        return 0;
}



int generateNumber(int min, int max){

    return (rand() % (max - min+1)) + min;
}
