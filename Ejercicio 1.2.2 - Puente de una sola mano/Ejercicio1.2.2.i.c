#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

#define autosNorte1 3
#define autosNorte2 13
#define autosNorte3 4

#define autosSur1 3
#define autosSur2 16
#define autosSur3 4

sem_t nortePasando, surPasando, puente;
pthread_mutex_t mutexPuenteN = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexPuenteS = PTHREAD_MUTEX_INITIALIZER;

/**
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del norte
 */ 
void vehiculoNorte(){
    
    //SECCION ENTRADA
    pthread_mutex_lock(&mutexPuenteN); //Esto se hace para que, si llegan dos autos a la vez desde el norte el primero solo bloquee el mutex
    if(sem_trywait(&nortePasando) == 0){//esto es, si hay un auto por el norte pasando actualmente
        sem_post(&nortePasando);
       
    }else{ //si no hay un auto por el norte pasando actualmente
        sem_wait(&puente);//si el puente esta disponible lo bloquea, sino espera a que otro lo desbloquee
    }
    
    //SECCION CRITICA
    printf("NORTE: Auto del norte comenzo a pasar. PATENTE: %i\n",gettid());
    sem_post(&nortePasando);
    pthread_mutex_unlock(&mutexPuenteN);
    sleep(1);
    printf("NORTE: Auto del norte termino de pasar. PATENTE: %i\n",gettid());
    sem_wait(&nortePasando);
    
    //SECCION SALIDA
    if(sem_trywait(&nortePasando) == 0){//si hay un auto pasando por el norte actualmente
        sem_post(&nortePasando);
    }else{ //si el ultimo en pasar es el auto actual...
        sem_post(&puente);
    }
    
    pthread_exit(NULL);
}

/**
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del sur
 */ 
void vehiculoSur(){
    //SECCION ENTRADA
    pthread_mutex_lock(&mutexPuenteS); //Esto se hace para que, si llegan dos autos a la vez desde el norte el primero solo bloquee el mutex
    if(sem_trywait(&surPasando) == 0){//esto es, si hay un auto por el norte pasando actualmente
        sem_post(&surPasando);
       
    }else{ //si no hay un auto por el norte pasando actualmente
        sem_wait(&puente);//si el puente esta disponible lo bloquea, sino espera a que otro lo desbloquee
    }
    
    //SECCION CRITICA
    printf("SUR: Auto del sur comenzo a pasar. PATENTE: %i \n",gettid());
    sem_post(&surPasando);
    pthread_mutex_unlock(&mutexPuenteS);
    sleep(1);
    printf("SUR: Auto del sur termino de pasar. PATENTE: %i\n",gettid());
    sem_wait(&surPasando);
    
    //SECCION SALIDA
    if(sem_trywait(&surPasando) == 0){//si hay un auto pasando por el norte actualmente
        sem_post(&surPasando);
    }else{ //si el ultimo en pasar es el auto actual...
        sem_post(&puente);
    }
    
    pthread_exit(NULL);
}
/*
 * Inicializa los semaforos e hilos con sus correspondientes valores.
 */ 
int main(){
    pthread_t threadSur[autosSur1 + autosSur2 + autosSur3];
    pthread_t threadNorte[autosNorte1 + autosNorte2 + autosNorte3];

    sem_init(&puente, 1, 1);
    sem_init(&nortePasando, 1, 0);
    sem_init(&surPasando, 1, 0);


    //Las creaciones de los hilos de los autos estan de la siguiente forma,
    //para simular que van llegando autos al puente mientras va transcurriendo el tiempo
    //Esto hace que no lleguen todos a la vez.
    for(int i = 0 ; i < autosSur1 ; i++){
        pthread_create(&(threadSur[i]), NULL, (void *)vehiculoSur, NULL);
    }

    for(int i = 0 ; i < autosNorte1 ; i++){
        pthread_create(&(threadNorte[i]), NULL, (void *)vehiculoNorte, NULL);
    }

    for(int i = 0 ; i < autosSur1 ; i++){
        pthread_join(threadSur[i], NULL);
    }

    for(int i = 0 ; i < autosNorte1 ; i++){
        pthread_join(threadNorte[i], NULL);
    }

    
    for(int i = 0 ; i < autosSur2 ; i++){
        pthread_create(&(threadSur[i]), NULL, (void *)vehiculoSur, NULL);
    }

    for(int i = 0 ; i < autosNorte2 ; i++){
        pthread_create(&(threadNorte[i]), NULL, (void *)vehiculoNorte, NULL);
    }

    for(int i = 0 ; i < autosSur2 ; i++){
        pthread_join(threadSur[i], NULL);
    }

    for(int i = 0 ; i < autosNorte2 ; i++){
        pthread_join(threadNorte[i], NULL);
    }
    
    
    for(int i = 0 ; i < autosSur3 ; i++){
        pthread_create(&(threadSur[i]), NULL, (void *)vehiculoSur, NULL);
    }

    for(int i = 0 ; i < autosNorte3 ; i++){
        pthread_create(&(threadNorte[i]), NULL, (void *)vehiculoNorte, NULL);
    }

    for(int i = 0 ; i < autosSur3 ; i++){
        pthread_join(threadSur[i], NULL);
    }

    for(int i = 0 ; i < autosNorte3 ; i++){
        pthread_join(threadNorte[i], NULL);
    }

}