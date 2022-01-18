#ifndef __COURSE_H_
#define __COURSE_H_

int faireDesTours( Voiture *shared_memory,  sem_t *semaphore, int i , unsigned int tempsMaxCircuit ) ;
int finale( Voiture *shared_memory, sem_t *semaphore, int i );
double generateNumber(void) ;
double generateStandStop(void);
bool goStand(unsigned int digit);
void goOut(int i,  Voiture *shared_memory) ;
#endif