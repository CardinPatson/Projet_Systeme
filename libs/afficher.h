#ifndef __AFFICHER_H_
#define __AFFICHER_H_
void afficherTableau(Voiture *shared_memory,Voiture *copyTableau, Session current_session, sem_t *semaphore, unsigned int tempsMaxCircuit , char *argv);
void sortLap(Voiture *copyTableau, Session current_session) ;
int compare(const void * a, const void * b);
int finished( Voiture *shared_memory, Session current_session, unsigned int tempsMaxCircuit , int nbre_tours_finale);
bool savedFile(Voiture *copyTableau,  char *argv , Session current_session);
#endif
