#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateNumberBetween25_40();

typedef struct {
	int numero
	char pilote
 	int vitesse	
}voiture 
//difffere sur le numero
// 1 course = 300km

int main(int argc , char **argv)
{
  printf ("A number between 25 and 40: %d\n", generateNumberBetween25_40() );

  return 0;
}

/* fonction qui va generer un nbr entre 25 et 40 (après ont va le convertir en minutes...) */
int generateNumberBetween25_40() {

  /* initialize random generator */
    srand ( time(NULL) );

  /* generate random numbers */ 	
    return (rand() % 15 + 25);
}
