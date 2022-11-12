#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <string.h>
#include "colores.h"

#define mq_key 1234
#define RECOLECTORES 3
#define CLASIFICADORES 2
#define RECICLADORES 4
#define RECOLECTOR "./Recolector"
#define CLASIFICADOR "./Clasificador"
#define RECICLADOR "./Reciclador"

int mq;

/*
 * Crea e inicializa las colas de mensajes.
 * Se utiliza para las MQ en msgsnd() y msgrcv() IPC_NOWAIT para evitar que se bloqueen 
 * los procesos al intentar enviar un mensaje en una MQ llena o intentar leer de una MQ
 * vacia y poder asi trazar mejor el programa.
 **/
void inicializarColaMensajes() {
	mq = msgget(mq_key, 0666);
    if (mq != -1) msgctl(mq, IPC_RMID, NULL); // Si la cola existe, la elimino.
    
    mq = msgget(1234, IPC_CREAT | 0666); // Se crea una nueva cola vacía.
	
	if (mq == -1) {
		printf("%sOcurrio un error al crear la cola de mensajes. %s\n", red(), reset());
	}
}

int main() {
	inicializarColaMensajes();

	for (int i = 0; i < RECOLECTORES; i ++) { // Se crean los recolectores.
		pid_t p_recolector;
		char * argv[512];
		argv[0] = RECOLECTOR;
		 	 
		p_recolector = fork();
		
		if (p_recolector == 0) {
			execvp(argv[0], argv);
			exit(0);
		}
	}

	sleep(1); // Se duerme el proceso padre para que los recolectores comiencen a producir antes de que los clasificadores consuman.

	for (int j = 0; j < CLASIFICADORES; j ++) { // Se crean los clasificadores.
		pid_t p_clasificador;
		char * argv[512];
		argv[0] = CLASIFICADOR;
		
		p_clasificador = fork();
		
		if (p_clasificador == 0) {
			execvp(argv[0], argv);
			exit(0);
		}
	}
	
	sleep(1); // Se duerme el proceso padre para que los clasificadores comiencen a producir antes de que los recicladores consuman.
	
	for (int k = 0; k < RECICLADORES; k ++) { // Se crean los recicladores.
		pid_t p_reciclador;
		char * argv[512];
		char argv_k[2];

		argv[0] = RECICLADOR;
		sprintf(argv_k, "%d", k);
		argv[1] = argv_k;
		
		p_reciclador = fork();
		
		if (p_reciclador == 0) {
			execvp(argv[0], argv);
			exit(0);
		}
	}
	
	while(wait(NULL) > 0); // El proceso padre espera a que todos los hijos terminen.
	
	msgctl(mq, IPC_RMID, NULL);

	return 0;
}
