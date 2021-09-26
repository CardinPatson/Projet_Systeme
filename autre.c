#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOMBRE 1

int generateNumber(void);


typedef struct {
        int numero;
        
        int *temps_S1;
        int *temps_S2;
        int *temps_S3;

        int tempsTotal;
} voiture;

voiture tabStuctVoiture[NOMBRE];



int main(void) {

    srand (time(NULL));

    int compteur = 0; // pour savoir si malloc() ou realloc()

    
    

    while(tabStuctVoiture[0].tempsTotal <= 5400) {

        if (compteur == 0)
        {
            for (size_t k = 0; k < NOMBRE; k++)
            {
                tabStuctVoiture[k].temps_S1 = calloc(1, sizeof(int));
                if (tabStuctVoiture[k].temps_S1 == NULL) {
                    perror("Problem calloc() !");
                    exit(EXIT_FAILURE);
                }
                tabStuctVoiture[k].temps_S1[0] = generateNumber();
                tabStuctVoiture[k].tempsTotal += tabStuctVoiture[k].temps_S1[0];
            }
            compteur++;
        }

        else
        {
            for (size_t k = 0; k < NOMBRE; k++)
            {
                tabStuctVoiture[k].temps_S1 = realloc(tabStuctVoiture[k].temps_S1, (compteur + 1) * sizeof(int));
                if (tabStuctVoiture[k].temps_S1 == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                tabStuctVoiture[k].temps_S1[compteur] = generateNumber();
                tabStuctVoiture[k].tempsTotal += tabStuctVoiture[k].temps_S1[compteur];
            }
            compteur++;
        }
    }


    printf("%d\n", compteur);
    printf("%d\n\n", tabStuctVoiture[0].tempsTotal);

    for(size_t i=0; i < NOMBRE; i++)
    {
        for(size_t j = 0; j < compteur; j++)
        {
        printf("%4d",tabStuctVoiture[i].temps_S1[j]);
        }
        printf("\n");
    }


    
    return 0;
}


int generateNumber(void){
  return (rand() % 14 + 26);
}


/*

    for(size_t i=0; i < NOMBRE; i++)
    {
        for(size_t j = 0; j < compteur; j++)
        {
        printf("%4d",tabStuctVoiture[i].temps[j]);
        }
        printf("\n");
    }
*/
