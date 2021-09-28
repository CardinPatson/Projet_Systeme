#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMBER_OF_CARS 1

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

    int cmpAlloc = 0;
    int numero = 0;


        if( (cmpAlloc == 0) && (tabStuctVoiture[0].tempsTotal <= 5400) )
        {
            /*   ****       S1     ****     */
            numero = generateNumber(25, 40);
            tabStuctVoiture[0].temps_S1 = calloc(1, sizeof(int));
            if (tabStuctVoiture[0].temps_S1 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[0].temps_S1[0] = numero;
            tabStuctVoiture[0].tempsTotal += numero;
            /* *************************************** */


            /*   ****       S2     ****     */
            numero = generateNumber(25, 40);
            tabStuctVoiture[0].temps_S2 = calloc(1, sizeof(int));
            if (tabStuctVoiture[0].temps_S2 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[0].temps_S2[0] = numero;
            tabStuctVoiture[0].tempsTotal += numero;
            /* *************************************** */


            /*   ****       S3     ****     */
            numero = generateNumber(25, 40);
            tabStuctVoiture[0].temps_S3 = calloc(1, sizeof(int));
            if (tabStuctVoiture[0].temps_S3 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[0].temps_S3[0] = numero;
            tabStuctVoiture[0].tempsTotal += numero;
            /* *************************************** */


            cmpAlloc++;
        }
        
        if ((cmpAlloc > 0) && (tabStuctVoiture[0].tempsTotal <= 5400)) {

            while ((cmpAlloc > 0) && (tabStuctVoiture[0].tempsTotal <= 5400))
            {
                if (tabStuctVoiture[0].tempsTotal >= 5400)
                {
                    break;
                }
                /*   ****       S1     ****     */
                numero = generateNumber(25, 40);
                tabStuctVoiture[0].temps_S1 = realloc(tabStuctVoiture[0].temps_S1, (cmpAlloc + 1) * sizeof(int));
                if (tabStuctVoiture[0].temps_S1 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                
                tabStuctVoiture[0].temps_S1[cmpAlloc] = numero;
                tabStuctVoiture[0].tempsTotal += numero;
                /* *************************************** */


                if (tabStuctVoiture[0].tempsTotal >= 5400)
                {
                    break;
                }
                /*   ****       S2     ****     */
                numero = generateNumber(25, 40);
                tabStuctVoiture[0].temps_S2 = realloc(tabStuctVoiture[0].temps_S2, (cmpAlloc + 1) * sizeof(int));
                if (tabStuctVoiture[0].temps_S2 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                
                tabStuctVoiture[0].temps_S2[cmpAlloc] = numero;
                tabStuctVoiture[0].tempsTotal += numero;
                /* *************************************** */


                if (tabStuctVoiture[0].tempsTotal >= 5400)
                {
                    break;
                }
                /*   ****       S3     ****     */
                numero = generateNumber(25, 40);
                tabStuctVoiture[0].temps_S3 = realloc(tabStuctVoiture[0].temps_S3, (cmpAlloc + 1) * sizeof(int));
                if (tabStuctVoiture[0].temps_S3 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                
                tabStuctVoiture[0].temps_S3[cmpAlloc] = numero;
                tabStuctVoiture[0].tempsTotal += numero;
                /* *************************************** */


                cmpAlloc++;
            }
        

        }

        else {
            printf("probleme !");
        }

        
        
        /*
        printf("%d", tabStuctVoiture[0].tempsTotal);

        for (int i = 0; i < 50; i++)
        {
            printf("%d\n", tabStuctVoiture[0].temps_S1[i]);
            printf("%d\n", tabStuctVoiture[0].temps_S2[i]);
            printf("%d\n", tabStuctVoiture[0].temps_S3[i]);
        }
        */




        free(tabStuctVoiture[0].temps_S1);
        free(tabStuctVoiture[0].temps_S2);
        free(tabStuctVoiture[0].temps_S3);



    


    return 0;
}

int generateNumber(int min, int max){

    return (rand() % (max - min+1)) + min;
}
