#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int generateNumberBetween25_40();
int generate();

typedef struct {
        int numero;
        char pilote[30];
        int vitesse;
        int *tempsS2;
        int *tempsS1;
        int *tempsS3;

}voiture;
//difffere sur le numero
// 1 course = 300km

int main(int argc , char **argv)
{
  generate();

  //Information sur la voiture

  voiture voiture[20];

  printf("Numero de voiture: \t");
  scanf("%d" , &voiture[0].numero);
  printf("Nom du pilote: \t");
  scanf("%s" , voiture[0].pilote);
  printf("Vitesse de la voiture: \t");
  scanf("%d" , &voiture[0].vitesse);

  printf("Pilote %s , voiture %d de vitesse %d pret pour l'essai libre\n\n" , voiture[0].pilote , voiture[0].numero , voiture[0].vitesse);

  //Essaie libre : 1h30 = 5400s
  //voiture[0].tempsS1 = (int*)malloc(1*sizeof(int));
  int temps_secteur1 = 0 ;
  int temps_secteur2 = 0 ;
  int temps_secteur3 = 0 ;
  int temps_secteur = 0 ;
  int count_secteur = 1;
  int countS1 = 0;
  int countS2 = 0;
  int countS3 = 0;
  //lors de l'initialisation du tableau ()
  while(temps_secteur<=5400){//1, 2 , 3

    if(count_secteur == 1){

      temps_secteur1 = generateNumberBetween25_40();
      temps_secteur += temps_secteur1 ;
 //     voiture[0].tempsS1[countS1] = temps_secteur1;
      countS1++;
//      voiture[0].tempsS1 = (int*)realloc(voiture[0].tempsS1 , count*sizeof(int));

      printf("secteur %d parcouru en %d secondes\n" , count_secteur , temps_secteur1) ;
    }
    if(count_secteur == 2){
      temps_secteur2 = generateNumberBetween25_40();
      temps_secteur += temps_secteur2 ;
      countS2++;
      printf("secteur %d parcouru en %d secondes\n" , count_secteur , temps_secteur2) ;
    }
    if(count_secteur == 3){
     temps_secteur3 = generateNumberBetween25_40() ;
     temps_secteur += temps_secteur3 ;
     printf("secteur %d parcouru en %d secondes\n\n" , count_secteur , temps_secteur3);
    }

    //temps_secteur = temps_secteur1 + temps_secteur2 + temps_secteur3;
    count_secteur++;//2 , 3 , 4
    if(count_secteur > 3){
      count_secteur = 1;
    }
  }
  printf("temps de secteur  total : %d\n" , temps_secteur);

  //Test
  printf ("A number between 25 and 40: %d\n", generateNumberBetween25_40() );
  printf("A new number between 25 and 40 : %d\n" , generateNumberBetween25_40()) ;
  return 0;
}

/* generer un nbr entre 25 et 40 (après ont va le convertir en minutes...) */

int generate(){

  /* initialize random generator srand doit etre appeléune seule fois */
    srand ( time(NULL) );

  /* generate random numbers */
  //NB quand on le melange avec srand il g�nere le nombre 1 fois pour tout le programme
 //  return (rand() % 15 + 25);
}

int generateNumberBetween25_40(){

  return (rand() % 15 +25);
//  return (rand()%(MAX - MIN +1) -MIN
}
