#!/bin/bash
enter=""
clear
echo -e "\n********BIENVENUE DANS CETTE NOUVELLE EDITION DE LA FORMULE 1**********\n"
echo -e "\n**********************LANCEMENT DES ESSAIES****************************\n"
read -p 'Appuyer la touche Enter pour lancer lessaie P1 ' enter
./main P1
clear
echo -e "\n***************CLASSEMENT DE P1 DANS LE FICHIER P1.txt*****************\n"
read -p 'Appuyer la touche Enter pour lancer lessaie P2 ' enter
./main P2
clear
echo -e "\n***************CLASSEMENT DE P2 DANS LE FICHIER P1.txt*****************\n"
read -p 'Appuyer la touche Enter pour lancer lessaie P3 ' enter
./main P3
clear
echo -e "\n***************CLASSEMENT DE P3 DANS LE FICHIER P3.txt******************\n"
read -p 'Appuyer la touche Enter pour acceder aux qualifications ' enter
clear
echo -e "\n*******************BIENVENUE AUX QUALIFICATIONS*************************\n"
read -p 'Appuyer la touche Enter pour lancer la Q1 ' enter
./main Q1
clear
echo -e "\n***************CLASSEMENT DE Q1 DANS LE FICHIER Q1.txt*****************\n"
read -p 'Appuyer la touche Enter pour lancer la Q2 ' enter
./main Q2
clear
echo -e "\n***************CLASSEMENT DE Q2 DANS LE FICHIER Q2.txt*****************\n"
read -p 'Appuyer la touche Enter pour lancer la Q3 ' enter
./main Q3
clear
echo -e "\n***************CLASSEMENT DE Q3 DANS LE FICHIER Q3.txt*****************\n"
read -p 'Appuyer la touche Enter pour acceder à la finale ' enter
clear
echo -e "\n**********************BIENVENUE A LA FINALE****************************\n"
read -p 'Appuyer la touche Enter pour lancer la finale ' enter
./main FINALE
clear
echo -e "\n********MERCI DAVOIR PARTICIPE A CETTE EDITION DE LA FORMULE 1**********\n"
echo -e "\n----- VOUS VERREZ LE CLASSEMENT FINALE DANS LE FICHIER FINALE.txt  -----\n"
echo -e ""
echo -e "\n-----                       A BIENTOT                              -----\n"
exit