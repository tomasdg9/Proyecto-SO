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


/**
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del norte
 */ 
void vehiculoNorte(){
    //sleep(2); Puedo hacer esto para hacer pasar primero a los del sur (el planificador hace pasar siempre a los del norte primero)
    if(sem_trywait(&nortePasando) != 0){//Si no hay un auto pasando por el norte, paso SOLO si el mutex no esta bloqueado
        pthread_mutex_lock(&mutexPuente); //bloqueo el puente, dejando pasar SOLO a los autos provenientes del norte, si esta pasando un auto del sur se queda esperando hasta que libere el mutex
    }//duda: Con un wait sobre un semaforo, es lo mismo?
    
    sem_wait(&semNorte);
    
    sem_post(&nortePasando);
    sem_post(&semNorte);
    printf("Vehiculo cruzando por el norte \n");
    sleep(2);
    printf("Vehiculo que cruzo por el norte, termino de pasar \n");
    sem_trywait(&nortePasando); //senala que vehiculo norte termino de pasar
    
    if(sem_trywait(&nortePasando) != 0){//esto es, si es el ultimo vehiculo pasando por el norte en ese instante, entonces libero el mutex
        pthread_mutex_unlock(&mutexPuente);
    }
}

/**
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del sur
 */ 
void vehiculoSur(){
    if(sem_trywait(&surPasando) != 0){//Si no hay un auto pasando por el sur, paso SOLO si el mutex no esta bloqueado
        pthread_mutex_lock(&mutexPuente); //bloqueo el puente, dejando pasar SOLO a los autos provenientes del norte, si esta pasando un auto del sur se queda esperando hasta que libere el mutex
    }//duda: Con un wait sobre un semaforo, es lo mismo?
    
    sem_wait(&semSur);
    
    sem_post(&surPasando);
    sem_post(&semSur);
    printf("Vehiculo cruzando por el sur \n");
    sleep(2);
    printf("Vehiculo que cruzo por el sur, termino de pasar \n");
    sem_trywait(&surPasando);
    
    if(sem_trywait(&surPasando) != 0){//esto es, si es el ultimo vehiculo pasando por el norte, entonces libero el mutex
        pthread_mutex_unlock(&mutexPuente);
    }
}
/*
 * Inicializa los semaforos e hilos con sus correspondientes valores.
 */ 
int main(){
    //semNorte=1, semSur=1, nortePasando = 0, surPasando = 0 mutexPuente = 1;
    pthread_t threadSur[autosSur1 + autosSur2 + autosSur3];
    pthread_t threadNorte[autosNorte1 + autosNorte2 + autosNorte3];

    sem_init(&semNorte, 1, 1);
    sem_init(&semSur, 1, 1);
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
