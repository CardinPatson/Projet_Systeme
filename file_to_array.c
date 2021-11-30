#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int main(void){
    FILE *myFile;
    myFile = fopen("./data/P1.txt", "r");

    //read file into array
    int numberArray[20];
    int i;

    for (i = 0; i < 20; i++)
    {
        fscanf(myFile, "%d", &numberArray[i]);
    }

    for (i = 0; i < 20; i++)
    {
        printf("Number is: %d\n\n", numberArray[i]);
    }

    return 0;
}
