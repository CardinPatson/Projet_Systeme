#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateNumberBetween25_40();

typedef struct {
	int numero;
	char pilote;
 	int vitesse;	
}voiture; 
//difffere sur le numero
// 1 course = 300km

int main(int argc , char **argv)
{
    /* initialize random generator */
    srand ( time(NULL) );

    for (size_t i = 0; i < 100; i++)
    {
        printf (": %d\n", generateNumberBetween25_40() );
    }
    
  

  return 0;
}

/* fonction qui va generer un nbr entre 25 et 40 (après ont va le convertir en minutes...) */
int generateNumberBetween25_40() {

  /* generate random numbers */ 	
    return (rand() % 15 + 25);
}
