#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h> 
#include <unistd.h> 

int generateNumberBetween25_40();

#define NOMBRE 20
typedef struct {
        int numero;
        int *tempsS1;
        int *tempsS2;
        int *tempsS3;
        int tempsTotal;
} voiture;






int main(int argc , char **argv)
{

    
  int tableauNumeroVoitures[NOMBRE] = { 44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99, 9, 47, 6, 63 };

  voiture voiture[NOMBRE]; /* initialize random generator srand doit etre appeléune seule fois */
    srand ( time(NULL) );


    int i;

    for (i = 0; i < 20; i++)
    {
        voiture[i].numero = tableauNumeroVoitures[i];
        
        printf("%d\n", voiture[i].numero);
    }


  

  //Essaie libre : 1h30 = 5400s
  int temps_secteur1 = 0 ;
  int temps_secteur2 = 0 ;
  int temps_secteur3 = 0 ;

  int temps_secteur = 0 ;
  int count_secteur = 1; // PERMET DE SAVOIR SUR QUEL SECTEUR ON SE TROUVE

  int countS1 = 0;
  int countS2 = 0;
  int countS3 = 0;

  int counts1_total = 1;
  int counts2_total = 1;
  int counts3_total = 1;
   //Mettre le temps de chaque secteur dans la structure de la voiture
  //lors de l'initialisation du tableau ()

  //PROBLEME LES 20 VOITURES DOIVENT TOURNER EN MEME TEMPS DONC NECESSITE UNE SYNCHRONISATION DE PROCESSUS

  while(temps_secteur <= 5400){//1, 2 , 3

    if(count_secteur == 1){

        temps_secteur1 = generateNumberBetween25_40();
        temps_secteur += temps_secteur1 ;//necessite un tableau de temps pour arreter chaque voiture a 5400s(un tableau de 20 tableau )

        if(counts1_total == 1 ){ 
          voiture[0].tempsS1 = calloc(1 , sizeof(int));
        }
        else{
          voiture[0].tempsS1 = realloc(voiture[0].tempsS1 , counts1_total*sizeof(int));
        }
        voiture[0].tempsS1[counts1_total-1] = temps_secteur1;
        //countS1++;

        printf("secteur %d parcouru en %d secondes\n" , count_secteur , temps_secteur1) ;
        counts1_total++;
    }


    if(count_secteur == 2){

      temps_secteur2 = generateNumberBetween25_40();
      temps_secteur += temps_secteur2 ;

      if(counts2_total == 1 ){
        voiture[0].tempsS2 = calloc(1 , sizeof(int));
      }
      else{ 
        voiture[0].tempsS2 = realloc(voiture[0].tempsS2 , counts2_total*sizeof(int)); 
      }
      voiture[0].tempsS2[counts2_total-1] = temps_secteur2;
      //countS2++;

      printf("secteur %d parcouru en %d secondes\n" , count_secteur , temps_secteur2) ;
      counts2_total++;
    }
    
    if(count_secteur == 3){
      temps_secteur3 = generateNumberBetween25_40() ;
      temps_secteur += temps_secteur3 ;
      if(counts3_total == 1 ){
        voiture[0].tempsS3 = calloc(1 , sizeof(int));
      }
      else{ 
        voiture[0].tempsS3 = realloc(voiture[0].tempsS3 , counts3_total*sizeof(int)); 
      }
      voiture[0].tempsS3[counts3_total-1] = temps_secteur3;    
      //countS3++;

      printf("secteur %d parcouru en %d secondes\n\n" , count_secteur , temps_secteur3);
      counts3_total++;
    }

    //temps_secteur = temps_secteur1 + temps_secteur2 + temps_secteur3;
    count_secteur++;//2 , 3 , 4
    if(count_secteur > 3){
      count_secteur = 1;
    }
  }
  //Test
  printf("counts1_total --> %d , counts2_total --> %d , counts3_total --> %d" , counts1_total , counts2_total , counts3_total);
  printf("temps de secteur  total : %d\n\n" , temps_secteur);
  printf("voiture 0 \n\n");
  
  //TEST TABLEAU TEMPS
  for(int j = 0 ; j<counts1_total ; j++){
    printf("passage numero %d sur le secteur 1  --> %d secondes\n" , j+1 , voiture[0].tempsS1[j]);
  }
  printf ("A number between 25 and 40: %d\n", generateNumberBetween25_40() );
  printf("A new number between 25 and 40 : %d\n" , generateNumberBetween25_40()) ;
  return 0;
}


int saveFile()
{
  int fd;
  int rc;
  char fileName[]="resultat.txt";
  char data[]="voiture 44 : 350 sec";
  mode_t mode = S_IRUSR | S_IWUSR;

 
  if ((fd=open(fileName,O_CREAT,mode)) < 0)
  {
    perror("Erreur lors du Open pour créer le fichier");
    return(0);
  }

  if (close(fd) < 0)
  {
    perror("Erreur lors du close.");
    return(0);  
  }

  if ((fd=open(fileName,O_RDWR,mode)) < 0)
  {
    perror("Erreur lors du Open pour écrire dans le fichier");
    return(0);  
  }
	//++ boucle pour parcourir le tableau des resultats et ecrire dans un fichier
  if (-1 == (rc=write(fd, data, strlen(data))))
  {
    perror("Erreur lors de l\'écriture dans le fichier");
    return(0);
  }

  if (0 != (rc=close(fd)))
  {
    perror("Erreur lors de la fermeture du fichier");
    return(0);
  }
}

int generateNumberBetween25_40(){

  return (rand() % 15 +25);
//  return (rand()%(MAX - MIN +1) -MIN
}
