
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>


// int main(int argc , char **argv){
//     int num[3] = {1,2};
//     char buffer[50];
//     int a = 10, b = 20, c;
//     c = a + b;
//     for(int i = 0 ; i<2 ; i++){
//         char temp[3] ;
//         sprintf(temp, "%d\n", num[i]);
//         strcat(buffer ,temp );
//     }
//     char d[2] = "5";
//     int e = d[0] ;
//     printf("valeur de b en int --> %d" , e);
 
//     // The string "sum of 10 and 20 is 30" is stored
//     // into buffer instead of printing on stdout
//     printf("%s", buffer);
 
//     return 0;
// }



int main()
{
  char tab[30] = "\033[33;1;4mHello\033[0m";


  printf("%s" , tab );
  return 0;
}

 // //INSERTION DES LIGNES DU FICHIER COURSE DANS NUMBERARRAY
    // // FILE *myFile;
    // // myFile = fopen(course, "r");
    // int myFile ;
   
    // // if (myFile == NULL) {
    // //     perror("fopen() failed !");
    // //     exit(EXIT_FAILURE);
    // // }
    // if((myFile = open(course , 0)) == -1){
    //         perror("open failed");
    //         exit(EXIT_FAILURE);
    // }
    // //read file into array
    // // for (int i = 0; i < current_session->total_cars; i++)
    // // {   
    //     // ssize_t r;
    //     // if((r = read(myFile , numberArray , sizeof(numberArray))) == -1){
    //     //     printf("error read");
    //     //     exit(EXIT_FAILURE);
    //     // }
        
    //     // fscanf(myFile, "%d", &numberArray[i]);
    // // }
    // //fclose(myFile);
    // close(myFile);
    // return 0;