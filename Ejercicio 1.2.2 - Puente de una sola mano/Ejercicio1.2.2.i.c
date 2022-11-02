#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>

#define autosNorte1 3
#define autosNorte2 3
#define autosNorte3 4

#define autosSur1 3
#define autosSur2 3
#define autosSur3 4

sem_t semNorte, semSur, nortePasando, surPasando;
pthread_mutex_t mutexPuente = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutexPuenteSur = PTHREAD_MUTEX_INITIALIZER;

int patente;

/**
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del norte
 */ 
void vehiculoNorte(){
    sem_wait(&semNorte); //Esto se hace para que, si llegan dos autos a la vez desde el norte el primero solo bloquee el mutex
    if(sem_trywait(&nortePasando) == 0){//esto es, si hay un auto por el norte pasando actualmente
        sem_post(&nortePasando);
        printf("NORTE: Ya hay un auto pasando en el norte. Pasa detras de el \n");
        sem_post(&nortePasando);
        sem_post(&semNorte);
        sleep(1);
        printf("NORTE: Auto del norte termina de pasar \n");
        sem_wait(&nortePasando);//termina de pasar entonces decrementa el semaforo
        
        if(sem_trywait(&nortePasando) == 0){//si hay un auto pasando por el norte actualmente
            sem_post(&nortePasando);
        }else{ //si el ultimo en pasar es el auto actual...
            pthread_mutex_unlock(&mutexPuente);
        }
        
    }else{ //si no hay un auto por el norte pasando actualmente
        pthread_mutex_lock(&mutexPuente);//si el puente esta disponible lo bloquea, sino espera a que otro lo desbloquee
        printf("NORTE: Auto del norte comenzo a pasar. \n");
        sem_post(&nortePasando);
        sem_post(&semNorte);
        sleep(1);
        printf("NORTE: Auto del norte termino de pasar. \n");
        sem_wait(&nortePasando);
        //ver si lo siguiente va afuera del else
        
        if(sem_trywait(&nortePasando) == 0){//si hay un auto pasando por el norte actualmente
            sem_post(&nortePasando);
        }else{ //si el ultimo en pasar es el auto actual...
            pthread_mutex_unlock(&mutexPuente);
        }
        
    }
    
    pthread_exit(NULL);
}

/**
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del sur
 */ 
void vehiculoSur(){
    sem_wait(&semSur);
    if(sem_trywait(&surPasando) == 0){//esto es, si hay un auto por el sur pasando actualmente
        sem_post(&surPasando);
        printf("SUR: Ya hay un auto pasando en el sur. Pasa detras de el \n");
        sem_post(&surPasando);
        sem_post(&semSur);
        sleep(1);
        printf("SUR: Auto del sur termina de pasar \n");
        sem_wait(&surPasando);//termina de pasar entonces decrementa el semaforo
        
        if(sem_trywait(&surPasando) == 0){//si hay un auto pasando por el sur actualmente
            sem_post(&surPasando);
        }else{ //si el ultimo en pasar es el auto actual...
            pthread_mutex_unlock(&mutexPuente);
        }
        
    }else{ //si no hay un auto por el norte pasando actualmente SE QUEDA ESPERANDO ACA PORQUE NO HAY NINGUNO PASANDO POR EL SUR 
        pthread_mutex_lock(&mutexPuente);//si el puente esta disponible lo bloquea, sino espera a que otro lo desbloquee
        printf("SUR: Auto del sur comenzo a pasar. \n");
        sem_post(&surPasando);
        sem_post(&semSur);
        sleep(1);
        printf("SUR: Auto del sur termino de pasar. \n");
        sem_trywait(&surPasando);
        //ver si lo siguiente va afuera del else
        
        if(sem_trywait(&surPasando) == 0){//si hay un auto pasando por el sur actualmente
            sem_post(&surPasando);
        }else{ //si el ultimo en pasar es el auto actual...
            pthread_mutex_unlock(&mutexPuente);
        }
    }
    pthread_exit(NULL);
}
/*
 * Inicializa los semaforos e hilos con sus correspondientes valores.
 */ 
int main(){
    //semNorte=1, semSur=1, nortePasando = 0, surPasando = 0 ;
    pthread_t threadSur[autosSur1 + autosSur2 + autosSur3];
    pthread_t threadNorte[autosNorte1 + autosNorte2 + autosNorte3];

    sem_init(&semNorte, 1, 1);
    sem_init(&semSur, 1, 1);
    sem_init(&nortePasando, 1, 0);
    sem_init(&surPasando, 1, 0);
    
    patente = 0;

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
