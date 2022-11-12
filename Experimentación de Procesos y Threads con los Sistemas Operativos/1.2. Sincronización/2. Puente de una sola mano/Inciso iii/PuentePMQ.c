#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <limits.h>
#include "colores.h"
#include "mensajes.h"

#define mq_key 1234
#define cantidadNORTE 25
#define cantidadSUR 25
#define VehiculoNORTE "./VehiculoNORTE"
#define VehiculoSUR "./VehiculoSUR"

int mq;

void inicializarMQ() {
    mq = msgget(mq_key, 0666);
    if (mq != -1) msgctl(mq, IPC_RMID, NULL); // Si la cola existe, la elimino.
    
    mq = msgget(mq_key, IPC_CREAT | 0666); // Se crea una nueva cola vacía.
	
	if (mq == -1) {
		printf("%sOcurrio un error al crear la cola de mensajes. %s\n", red(), reset());
	}
    
    MENSAJE S1 = crearMensaje(Puente);
    msgsnd(mq, &S1, SIZE_MENSAJE, 0);
    
    MENSAJE S2 = crearMensaje(MutexN);
    msgsnd(mq, &S2, SIZE_MENSAJE, 0);
    
    MENSAJE S3 = crearMensaje(MutexS);
    msgsnd(mq, &S3, SIZE_MENSAJE, 0);
    
    for (int i = 0; i < CONTADOR_PUENTE; i ++) {
        MENSAJE S4 = crearMensaje(CoordinadorN);
        msgsnd(mq, &S4, SIZE_MENSAJE, 0);
    }
}

int main(){
    inicializarMQ();
    
    for (int i = 0 ; i < cantidadNORTE ; i++){
        pid_t p_norte;
        char * argv[PATH_MAX];
        
        argv[0] = VehiculoNORTE;
        p_norte = fork();
        
        if (p_norte == 0) {
            execvp(argv[0], argv);
            exit(0);
        }
    }
    
    for (int i = 0 ; i < cantidadSUR; i++){
        pid_t p_sur;
        char * argv[PATH_MAX];
        
        argv[0] = VehiculoSUR;
        p_sur = fork();
        
        if (p_sur == 0) {
            execvp(argv[0], argv);
            exit(0);
        }
    }
    
    while(wait(NULL) > 0);
    
    msgctl(mq, IPC_RMID, NULL);
    
    return 0;
}
