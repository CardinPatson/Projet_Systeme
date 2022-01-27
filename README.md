# Projet Système TP Janvier 2022

Executer le projet : compiler le fichier exec.sh dans le dossier

### LANCEMENT DE LA COURSE :

executez le fichier lancement.sh (**./bin/lancement.sh**)dans le dossier bin/ ;

## Description

Le but du projet est de gérer un week-end complet d’un grand prix de Formule 1,
depuis les séances d’essais du vendredi jusqu’à la course du dimanche, en
passant par les essais du samedi et la séance de qualifications. Il y a 20
voitures engagées dans un grand prix.

Leurs numéros sont : 44, 77, 11, 33, 3, 4, 5, 18, 14, 31, 16, 55, 10, 22, 7, 99,
9, 47, 6, 63.

Un circuit de F1 est divisé en 3 secteurs (S1, S2, S3).

## Le calendrier d’un week-end de F1 est établi comme suit :

- Vendredi matin, une séance d’essais libres d’1h30 (P1)
- Vendredi après-midi, une séance d’essais libres d’1h30 (P2)
- Samedi matin, une séance d’essais libres d’1h (P3)
- Samedi après-midi, la séance de qualifications, divisée en 3 parties :

- Q1, durée 18 minutes, qui élimine les 5 dernières voitures (qui occuperont les
  places 16 à 20 sur la grille de départ de la course)
- Q2, durée 15 minutes, qui élimine les 5 voitures suivantes (qui occuperont les
  places 11 à 15 sur la grille de départ de la course)
- Q3, durée 12 minutes, qui permet de classer les 10 voitures restantes pour
  établir les 10 premières places sur la grille de départ de la course
- Dimanche après-midi, la course en elle-même.

Votre projet devra prendre en charge les choses suivantes.

**Première partie : gestion des séances d’essai, des qualifications et de la
course**

## Lors des séances d’essais (P1, P2, P3) :

- Relever les temps dans les 3 secteurs à chaque passage pour chaque voiture
- Toujours savoir qui a le meilleur temps dans chacun des secteurs
- Classer les voitures en fonction de leur tour complet le plus rapide
- Indiquer la différence de temps avec la voiture qui précède
- Savoir si une voiture est aux stands (P)
- Savoir si une voiture est out (abandon de la séance)
- Dans ces 2 derniers cas, on conserve toujours le meilleur temps de la voiture
  et celle-ci reste dans le classement
- Conserver sur fichier le classement final à la fin de chaque séance

## Lors des qualifications (Q1, Q2, Q3) :

- Relever les temps dans les 3 secteurs à chaque passage pour chaque voiture
- Toujours savoir qui a le meilleur temps dans chacun des secteurs
- Classer les voitures en fonction de leur tour complet le plus rapide
- Indiquer la différence de temps avec la voiture qui précède
- Savoir si une voiture est aux stands (P)
- Savoir si une voitures est out (abandon de la séance)
- Dans ces 2 derniers cas, on conserve toujours le meilleur temps de la voiture
  et celle-ci reste dans le classement
- A la fin de Q1, il reste 15 voitures qualifiées pour Q2 et les 5 dernières
  sont placées à la fin de la grille de départ (places 16 à 20)
- A la fin de Q2, il reste 10 voitures qualifiées pour Q3 et les 5 dernières
  sont placées dans les places 11 à 15 de la grille de départ
- Le classement de Q3 attribue les places 1 à 10 de la grille de départ
- Conserver sur fichier le classement final à la fin des 3 séances (ce sera
  l’ordre de départ pour la course)

**Attention ! Lors des essais et des qualifications, les voitures roulent quand
elles veulent. Il n’y a pas un nombre déterminé de tours à faire.**

## Lors de la course : (un nombre déterminé de tours !!!)

- Le premier classement est l’ordre sur la grille de départ
- Le classement doit toujours être maintenu tout au long de la course (gérer les
  dépassements)
- Indiquer la différence de temps avec la voiture qui précède
- Relever les temps dans les 3 secteurs à chaque passage pour chaque voiture
- Toujours savoir qui a le meilleur temps dans chacun des secteurs
- Toujours savoir qui a le tour le plus rapide
- Savoir si la voiture est out (abandon) ; dans ce cas, elle sera classée en fin
  de classement
- Savoir si la voiture est aux stands (PIT), gérer le temps aux stands et faire
  ressortir la voiture à sa place dans la course (généralement 2 ou 3 PIT par
  voitures)
- Conserver sur fichier le classement final et le tour le plus rapide

**Remarque : les stands se trouvent toujours dans le secteur 3.**

De plus, il vous est demandé de **paramétrer votre programme.\***

En effet, les circuits peuvent être de longueur très variable et, dès lors, le
nombre de tours pour la course varie également (on essaie que le nombre total de
kilomètres soit toujours plus ou moins le même pour chacune des courses du
calendrier : environ 300-350 km).

## On vous demande :

- De réaliser le programme en C sous Linux
- D’utiliser la mémoire partagée comme moyen de communication inter-processus
- D’utiliser les sémaphores pour synchroniser l’accès à la mémoire partagée

Bon courage.
