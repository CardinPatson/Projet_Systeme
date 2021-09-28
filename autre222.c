#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOMBRE 20 

int generateNumber(int min, int max);


typedef struct {
        /* int numero; */

        int *temps_S1;
        int *temps_S2;
        int *temps_S3;

        int tempsTotal;
} voiture;

voiture tabStuctVoiture[NOMBRE];



int main(void) {

    srand (time(NULL));

    int cmpAlloc = 0;
    

    for (int a = 0; a < NOMBRE; a++) //a = 0    pour acceder au temps total de chaque voiture 
    {
        tabStuctVoiture[a].tempsTotal = 0;

        int numero = 0;

        if (tabStuctVoiture[a].tempsTotal == 0 && cmpAlloc == 0)
        {
            /*          S1          */
            numero = generateNumber(25, 40);
            tabStuctVoiture[a].tempsTotal += numero;

            tabStuctVoiture[a].temps_S1 = calloc(1, sizeof(int));
            if (tabStuctVoiture[a].temps_S1 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[a].temps_S1[cmpAlloc] = numero;


            /*          S2          */
            numero = generateNumber(25, 40);
            tabStuctVoiture[a].tempsTotal += numero;

            tabStuctVoiture[a].temps_S2 = calloc(1, sizeof(int));
            if (tabStuctVoiture[a].temps_S2 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[a].temps_S2[cmpAlloc] = numero;


            /*          S3          */
            numero = generateNumber(25, 40);
            tabStuctVoiture[a].tempsTotal += numero;

            tabStuctVoiture[a].temps_S3 = calloc(1, sizeof(int));
            if (tabStuctVoiture[a].temps_S3 == NULL) {
                perror("Problem calloc() !");
                exit(EXIT_FAILURE);
            }
            tabStuctVoiture[a].temps_S3[cmpAlloc] = numero;

        }
        cmpAlloc++;


        if (tabStuctVoiture[a].tempsTotal > 0 && cmpAlloc > 0)
        {
            while (tabStuctVoiture[a].tempsTotal <= 5400)
            {
                for (int i = 0; i < NOMBRE; i++)
                { //BOUCLE QUI FOUE LA MERDE
                
                    /*          S1          */
                    numero = generateNumber(25, 40);
                    tabStuctVoiture[a].tempsTotal += numero;

                    tabStuctVoiture[a].temps_S1 = realloc(tabStuctVoiture[a].temps_S1, (cmpAlloc + 1) * sizeof(int));
                    if (tabStuctVoiture[a].temps_S1 == NULL) {
                        perror("Problem realloc() !");
                        exit(EXIT_FAILURE);
                    }
                    tabStuctVoiture[a].temps_S1[cmpAlloc] = numero;


                    /*          S2          */
                    numero = generateNumber(25, 40);
                    tabStuctVoiture[a].tempsTotal += numero;

                    tabStuctVoiture[a].temps_S2 = realloc(tabStuctVoiture[a].temps_S2, (cmpAlloc + 1) * sizeof(int));
                    if (tabStuctVoiture[a].temps_S2 == NULL) {
                        perror("Problem realloc() !");
                        exit(EXIT_FAILURE);
                    }
                    tabStuctVoiture[a].temps_S2[cmpAlloc] = numero;


                    /*          S3          */
                    numero = generateNumber(25, 40);
                    tabStuctVoiture[a].tempsTotal += numero;

                    tabStuctVoiture[a].temps_S3 = realloc(tabStuctVoiture[a].temps_S3, (cmpAlloc + 1) * sizeof(int));
                    if (tabStuctVoiture[a].temps_S3 == NULL) {
                        perror("Problem realloc() !");
                        exit(EXIT_FAILURE);
                    }
                    tabStuctVoiture[a].temps_S3[cmpAlloc] = numero;
                }
                cmpAlloc++;

            }
        }

    }

    for (int i = 0; i < NOMBRE; i++)
    {
        printf("%d\n", tabStuctVoiture[i].tempsTotal);
    }



    return 0;
}



int generateNumber(int min, int max){

    return (rand() % (max - min+1)) + min;
}



/*
    for (int i = 0; i < NOMBRE; i++)
    {
        int tempsTotal = 0;

        while (tempsTotal <= 5400)
        {
            int numero = generateNumber(25, 40);
            tempsTotal += numero;
        }
        printf("Temps total: %d\n", tempsTotal);
    }

*/
