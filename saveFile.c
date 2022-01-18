#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>        
#include <stdlib.h>      
#include <fcntl.h>     
#include <unistd.h>  


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
