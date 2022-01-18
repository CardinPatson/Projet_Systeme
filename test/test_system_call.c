#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<fcntl.h>
#include<errno.h>

#include <stdbool.h>

#include <unistd.h>

#define O_RDONLY 0x0000      /* open for reading only */
#define O_WRONLY 0x0001
#define O_CREAT 0x0040
#define O_TRUNC 0x0200


extern int errno;

bool isRaceExist(char course[]) {

    char destination[20] = "./data/";
    strcat(destination, course);
    strcat(destination, ".txt");

    int fd = open(destination, O_RDONLY);

    if (fd ==-1) {
        return false;
    }

    close(fd);
    return true;

}

char tab[10] = {1 ,2,3,4,5,6,7 , 8 , 9};

int savedFile(char *argv[]) {
    char fichiertxt[20] = "./data/";
    strcat(fichiertxt, argv[1]);
    strcat(fichiertxt, ".txt");

    int fd = open(fichiertxt, O_WRONLY|O_TRUNC|O_APPEND);
    if (fd == -1) {
        perror("open() failed !");
        return false;
    }

    // char a_ecrire[100];
    // for (int i = 0; i < ; ++i) {//current_session.total_cars

    //     a_ecrire[i] = copyTableau[i].id;
    // }

    for(int i = 0 ; i < sizeof(tab) ; i++  ){
        if (write(fd, &tab[i] , sizeof(tab)) == -1){ //a_ecrire
        perror("Probleme avec le Write() !");
        return false;
        }

    }
    
    if(close(fd) == -1) { // on a fini de travailler avec le fichier shell
        perror("Fermeture du fichier");
        return false;
    }
    return 0;
}


int main(int argc , char **argv)
{
    printf("%d", isRaceExist("RA44CE.txt"));
    for (int i = 0 ; i< sizeof(tab) ; i++ ){
        printf("tab -- %d" ,tab[i] );
    }
    savedFile(argv);
    return 0;
}