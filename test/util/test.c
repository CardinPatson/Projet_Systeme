
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc , char **argv){
    int num[3] = {1,2};
    char buffer[50];
    int a = 10, b = 20, c;
    c = a + b;
    for(int i = 0 ; i<2 ; i++){
        char temp[3] ;
        sprintf(temp, "%d\n", num[i]);
        strcat(buffer ,temp );
    }
    char d[2] = "5";
    int e = d[0] ;
    printf("valeur de b en int --> %d" , e);
 
    // The string "sum of 10 and 20 is 30" is stored
    // into buffer instead of printing on stdout
    printf("%s", buffer);
 
    return 0;
}