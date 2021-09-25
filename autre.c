#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateNumber(void);


int main()
{
    srand ( time(NULL) );


    int *tempsS1 = NULL;

    int compteur = 0;
    int temps_secteur = 0;


    while(temps_secteur <= 5400)
    {
        if (compteur == 0)
        {
            tempsS1 = calloc(1, sizeof(int));
            if (!tempsS1) {
                perror("Problem Calloc() !");
                exit(EXIT_FAILURE);
            }

            tempsS1[0] = generateNumber();
            temps_secteur += tempsS1[0];
            compteur++;
        }

        else
        {
            tempsS1 = realloc(tempsS1, (compteur + 1) * sizeof(int));
            if (!tempsS1) {
                perror("Problem Realloc()");
                exit(EXIT_FAILURE);
            }

            tempsS1[compteur] = generateNumber();
            temps_secteur += tempsS1[compteur];
            compteur++;
        }
    }

    int Length  = sizeof(tempsS1)/sizeof(int);


    for (size_t i = 0; i < compteur; i++)
    {
        printf("%d\n", tempsS1[i]);
    }
    
  
    return (0);
}


int generateNumber(void){
  return (rand() % 14 + 26);
}

