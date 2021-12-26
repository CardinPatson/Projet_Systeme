#include<stdlib.h>
#include<string.h>

#include<stdio.h>
#include<fcntl.h>
#include<errno.h>

#include <stdbool.h>

#include <unistd.h>

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

bool savedFile(char *argv[]) {
    char fichiertxt[20] = "./data/";
    strcat(fichiertxt, argv[1]);
    strcat(fichiertxt, ".txt");

    int fd = open(fichiertxt, O_WRONLY | O_CREAT);
    if (fd == -1) {
        perror("open() failed !");
        return false;
    }

    for (int i = 0; i < current_session.total_cars; ++i) {

        if (write(fd, &copyTableau[i].id, sizeof (copyTableau[i].id)) == -1){
            perror("Probleme avec le Write() !");
            return false;
        }
    }

    if(close(fd) == -1) { // on a fini de travailler avec le fichier shell
        perror("Fermeture du fichier");
        return false;
    }
    return true;
}


int main()
{
    printf("%d", isRaceExist("RA44CE.txt"));
    return 0;
}
