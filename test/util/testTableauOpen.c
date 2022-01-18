#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc , char **argv){

int i, j, p, pip[2], o, sta, ex;
pid_t cpid;
uid_t uid;
system("clear");

if(argc == 1)
{
	printf("not enough arguments\n");
	exit(-1);
}

if((p = pipe(pip)) == -1) // test ou <0
{
	printf("erreur pipe\n");
	exit(-2);
}


char chaineBin[] = "/usr/bin/";
strcat(chaineBin , argv[1]);

if((o = open(chaineBin , 0)) == -1)
{

	char chaineBin[] = "/usr/sbin/";
	strcat(chaineBin , argv[1]);

	if((o = open(chaineBin , 0)) == -1)
	{	for(int j = 1 ; j<argc ; j++){
		int s;
		if((s = open(argv[j] , 0)) != -1)
		{

			struct stat st;
			if((sta = stat(argv[j] , &st)) == -1)
			{
				printf("error stat\n");
				exit(-3);
			}

			/*PREMIER TEST : RENVOIE LES CARACTERES DU FICHIERS SHELL*/
//			printf("\nNombre de ligne fichier shell %ld \n" , st.st_size);
			/*REUSSIE*/

			int c=0 ;
			int n=0;
			char *tab;
			tab = malloc(st.st_size*sizeof(char));

			if((read(s , tab , st.st_size)) == -1)
			{
				printf("error read\n");
				exit(0);
			}

			close(s);

			/*DEUXIEME TEST : RENVOIE TOUT LE CONTENU DU SHELL SOUS FORME DE TABLEAU*/
//			 printf("contenu du fichier shell : \n%s\n\n",tab);
			/*AVEC st.st_size COMME LONGUEUR : ECHEC  , AVEC 255 REUSSIE , AVEC REALLOC REUSSIE*/


			int i=0 ;
			char *ligne;
			ligne  = strtok(tab , "\n");

			/*TROISIEME TEST : RENVOIE LA VALEUR DE LIGNE*/
//			printf("valeur de ligne:\n %s\n\n" , ligne);
			/*ME RENVOIE UNIQUEMENT LA PREMIERE LIGNE DE MON FICHIER SHELL*/


			char **tableauLigne;
			tableauLigne = (char**)malloc(1*sizeof(char*));
			while(ligne != NULL)
			{

			tableauLigne[i] = ligne;
			i++;
			tableauLigne = realloc(tableauLigne , (i+1)*sizeof(char*));
			ligne = strtok(NULL , "\n");
			}

			/*QUATRIEME TEST : RENVOIE LE TABLEAU DE LIGNE ET SON NUMERO*/
//			for(int f=0 ; f<i ; f++){ printf("contenu de ligne %d : %s\n\n" ,f+1, tableauLigne[f]);}
			/*REUSSIE AVEC 255 COMME LONGUEUR DU TABLEAU*/


			for(int j =0 ; j<i ; j++)
			{
				int p = 0 ;
				int pip2[2] = {0};

				if((p = pipe(pip2)) == -1)
				{
					printf("erreur pipe\n");
					exit(-2);
				}

				/*TEST SUR LE FORK*/
				cpid = fork();


				if(cpid == 0)
				{

					close(1);

					if(dup(pip2[1]) == -1)
					{
						printf("error dup\n");
						exit(-5);
					}

					close(pip2[0]);

					char *commande;
					commande  = strtok(tableauLigne[j] , " ");

					char **tableauCommande = {0};
					tableauCommande = (char**)malloc(1*sizeof(char*));

					int k=0;
					while(commande  != NULL)
					{

						tableauCommande[k] = commande;
						k++;
						tableauCommande = realloc(tableauCommande , (k+1)*sizeof(char*));
						commande = strtok(NULL , " ");

					}
					tableauCommande[k+1] = NULL;

					if((ex = execvp(tableauCommande[0] , tableauCommande)) ==-1)
					{
						printf("error exec\n");
						exit(-6);
					}
				}
				else
				{
					close(pip2[1]);
					char buff[5000];
					ssize_t r;
					if((r=read(pip2[0] , buff ,sizeof(buff))) == -1 )
					{
						printf("error read \n");
						exit(0);
					}
					write(1 , buff ,r );
				}
			}
		}}
		return 0;
	}
}


if((cpid = fork()) == -1)
{
	printf("error fork\n");
	exit(-3);
}


if(cpid == 0 ) 
{
	if((dup(pip[1])) == -1)
	{
		printf("error dup\n");
		exit(-4);
	}

	close(pip[0]);//fermer la sortie(bout de lecture)

	char *tabArg[argc-1];

	for(int i =1 ; i<=argc ; i++)
	{
		tabArg[i-1] = argv[i];
	}

	if((ex = execv(chaineBin , tabArg)) == -1)
	{
		printf("error exec\n");
		exit(-5);
	}
}


else
{
	char buff[2000];
	ssize_t r;
	close(pip[1]); //FERMER LENTREE

	if((r = read(pip[0], buff, sizeof(buff))) == -1)
	{
		printf("error read\n" );
		exit(-6);
	}

	write(1 , buff , r);
	exit(0);
}

}




