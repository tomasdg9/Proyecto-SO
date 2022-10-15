#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>

sem_t semA1, semA2, semB, semC, semDE, semF;

/*
 * Muestra la letra A de la secuencia por pantalla. 
 **/
void * A() {
	while(1) {
		sem_wait(&semA1);
		sem_wait(&semA2);
		printf("A");
		fflush(stdout);
		sem_post(&semB);
		sem_post(&semC);
		sleep(1);
	}
}

/*
 * Muestra la letra B de la secuencia por pantalla. 
 **/
void * B() {
	while(1) {
		sem_wait(&semB);
		sem_wait(&semB);
		printf("B");
		fflush(stdout);
		sem_post(&semA1);
		sem_post(&semDE);
		sleep(1);
	}
}

/*
 * Muestra la letra C de la secuencia por pantalla. 
 **/
void * C() {
	while(1) {
		sem_wait(&semC);
		sem_wait(&semC);
		printf("C");
		fflush(stdout);
		sem_post(&semDE);
		sleep(1);
	}
}

/*
 * Muestra la letra D de la secuencia por pantalla. 
 **/
void * D() {
	while (1) {
		sem_wait(&semDE);
		printf("D");
		fflush(stdout);
		sem_post(&semF);
		sem_post(&semA2);
		sleep(1);
	}
}

/*
 * Muestra la letra E de la secuencia por pantalla. 
 **/
void * E() {
	while(1) {
		sem_wait(&semDE);
		printf("E");
		fflush(stdout);
		sem_post(&semF);
		sem_post(&semA2);
		sleep(1);
	}
}

/*
 * Muestra la letra F de la secuencia por pantalla. 
 **/
void * F() {
	while(1) {
		sem_wait(&semF);
		sem_wait(&semF);
		printf("F");
		fflush(stdout);
		sem_post(&semA1);
		sleep(1);
	} 
}

/*
 * Inicializa los semaforos con los valores adecuados para la sincronizacion.
 **/
void inicializarSemaforos() {
	sem_init(&semA1, 1, 1);
	sem_init(&semA2, 1, 1);
	sem_init(&semB, 1, 1);
	sem_init(&semC, 1, 0);
	sem_init(&semDE, 1, 0);
	sem_init(&semF, 1, 0);
}

int main () {
	pthread_t thread[6];
	
	inicializarSemaforos();
	
	pthread_create(&(thread[0]),NULL,(void*)A,NULL);
	pthread_create(&(thread[1]),NULL,(void*)B,NULL);
	pthread_create(&(thread[2]),NULL,(void*)C,NULL);
	pthread_create(&(thread[3]),NULL,(void*)D,NULL);
	pthread_create(&(thread[4]),NULL,(void*)E,NULL);
	pthread_create(&(thread[5]),NULL,(void*)F,NULL);
	
	for (int i = 0; i < 6; i++){
		pthread_join(thread[i],NULL);
	}
	
	return 0;
}
