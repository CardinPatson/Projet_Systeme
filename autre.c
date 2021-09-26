#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateNumber(void);


typedef struct {
        int numero;
        int *temps;
} voiture;

voiture tabStuctVoiture[3];



int main(void) {

    srand (time(NULL));
    int compteur = 0;
    int temps_secteur = 0;

    while(temps_secteur <= 5400) {

        if (compteur == 0)
        {
            for (size_t k = 0; k < 3; k++)
            {
                tabStuctVoiture[k].temps = calloc(1, sizeof(int));
                if (tabStuctVoiture[k].temps == NULL) {
                    perror("Problem calloc() !");
                    exit(EXIT_FAILURE);
                }
                tabStuctVoiture[k].temps[0] = generateNumber();
                temps_secteur += tabStuctVoiture[k].temps[0];
            }
            compteur++;
        }

        else
        {
            for (size_t k = 0; k < 3; k++)
            {
                tabStuctVoiture[k].temps = realloc(tabStuctVoiture[k].temps, (compteur + 1) * sizeof(int));
                if (tabStuctVoiture[k].temps == NULL) {
                    perror("Problem realloc() !");
                    exit(EXIT_FAILURE);
                }
                tabStuctVoiture[k].temps[compteur] = generateNumber();
                temps_secteur += tabStuctVoiture[k].temps[compteur];
            }
            compteur++;
        }
    }


    for(size_t i=0; i < 3; i++)
    {
        for(size_t j = 0; j < compteur; j++)
        {
        printf("%4d",tabStuctVoiture[i].temps[j]);
        }
        printf("\n");
    }
    
    return 0;
}


int generateNumber(void){
  return (rand() % 14 + 26);
}


/*

        printf("%d\n\n", compteur);
        for(size_t j = 0; j < compteur; j++)
        {
        printf("%4d",tabStuctVoiture[1].temps[j]);
        }
        printf("\n");
*/
