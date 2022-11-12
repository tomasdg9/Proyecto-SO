#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include "colores.h"

#define _GNU_SOURCE
#define cantidadNORTE 25
#define cantidadSUR 25

sem_t pasandoN;
sem_t pasandoS;
sem_t puente;
pthread_mutex_t mutexN;
pthread_mutex_t mutexS;

pid_t gettid(void);

/*
 * Realiza las acciones de los vehiculos provenientes del NORTE al cruzar el puente. 
 **/
void vehiculoNorte(){
    while(1) {
        //SECCION ENTRADA
        pthread_mutex_lock(&mutexN); // Si llegan dos autos del NORTE, solo el primero obtiene el mutex.
        if(sem_trywait(&pasandoN) == 0){ // Si hay autos del NORTE pasanado actualmente.
            sem_post(&pasandoN);
        }else{ // Si NO hay autos del NORTE pasando actualmente.
            sem_wait(&puente); // Bloquea el puente o espera para poder obtener el bloqueo.
        }
        sem_post(&pasandoN);
        pthread_mutex_unlock(&mutexN);
                
        //SECCION CRITICA
        printf("%sNORTE: Auto del norte pasando el puente. PATENTE: %i\n %s", blue(), gettid(), reset());
        sleep(1);
        
        //SECCION SALIDA
        pthread_mutex_lock(&mutexN);
        sem_wait(&pasandoN);
        if(sem_trywait(&pasandoN) == 0){ // Si quedan autos del NORTE pasando todavía.
            sem_post(&pasandoN);
        }else{ // Si es el ultimo auto del NORTE en pasar, libera el bloqueo del puente.
            sem_post(&puente);
        }
        pthread_mutex_unlock(&mutexN);
    }
    
    pthread_exit(EXIT_SUCCESS);
}

/*
 * Realiza las acciones de los vehiculos provenientes del SUR al cruzar el puente. 
 **/
void vehiculoSur(){
    while(1) {
        //SECCION ENTRADA
        pthread_mutex_lock(&mutexS); // Si llegan dos autos del SUR, solo el primero obtiene el mutex.
        if(sem_trywait(&pasandoS) == 0){ // Si hay autos del SUR pasando actualmente.
            sem_post(&pasandoS);
        }else{ // Si NO hay autos del SUR pasando actualmente.
            sem_wait(&puente); // Bloquea el puente o espera para poder obtener el bloqueo.
        }
        sem_post(&pasandoS);
        pthread_mutex_unlock(&mutexS);
                
        //SECCION CRITICA
        printf("%sSUR: Auto del sur pasando el puente. PATENTE: %i\n %s", red(), gettid(), reset());
        sleep(1);
        
        //SECCION SALIDA
        pthread_mutex_lock(&mutexS);
        sem_wait(&pasandoS);
        if(sem_trywait(&pasandoS) == 0){ // Si quedan autos del SUR pasando todavía.
            sem_post(&pasandoS);
        }else{ // Si es el ultimo auto del SUR en pasar, libera el bloqueo del puente.
            sem_post(&puente);
        }
        pthread_mutex_unlock(&mutexS);
    }
    
    pthread_exit(EXIT_SUCCESS);
}

void inicializarSemaforos() {
    sem_init(&puente, 0, 1);
    sem_init(&pasandoN, 0, 0);
    sem_init(&pasandoS, 0, 0);
    pthread_mutex_init(&mutexN, NULL);
    pthread_mutex_init(&mutexS, NULL);
}

void destruirSemaforos() {
    sem_destroy(&puente);
    sem_destroy(&pasandoN);
    sem_destroy(&pasandoS);
    pthread_mutex_destroy(&mutexN);
    pthread_mutex_destroy(&mutexS);
}

int main(){
    pthread_t threadNorte[cantidadNORTE];
    pthread_t threadSur[cantidadSUR];
    
    inicializarSemaforos();
    
    for(int i = 0 ; i < cantidadNORTE ; i ++){
        pthread_create(&(threadNorte[i]), NULL, (void *)vehiculoNorte, NULL);
    }

    for(int j = 0 ; j < cantidadNORTE; j ++){
        pthread_create(&(threadSur[j]), NULL, (void *)vehiculoSur, NULL);
    }
    
    for(int i = 0 ; i < cantidadSUR ; i ++){
        pthread_join(threadNorte[i], NULL);
    }
    
    for(int j = 0 ; j < cantidadSUR ; j ++){
        pthread_join(threadSur[j], NULL);
    }
    
    destruirSemaforos();
    
    return 0;
}
