#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include "colores.h"

#define mq_key 1234
#define SIZE_MENSAJE sizeof(MENSAJE) - sizeof(long)
#define VIDRIO 2
#define CARTON 3
#define PLASTICO 4
#define ALUMINIO 5

int mq;

/* mq -> Message Queue utilizada para toda la planta de reciclado.
 * -> Tipos de items
 * Tipo 1: Items recolectados en espera de clasificacion.
 * Tipo 2: Items clasificados del tipo Vidrio.
 * Tipo 3: Items clasificados del tipo Carton.
 * Tipo 4: Items clasificados del tipo Plastico.
 * Tipo 5: Items clasificados del tipo Aluminio.
 */

/*
 * struct utilizado para la representacion de los elementos de basura.
 * Son utilizados como Mensajes en las operaciones de las MQs.
 **/
typedef struct mensaje {
	long tipo;
	char nombre[10];
} MENSAJE;

/*
 * Clasifica los items de tipo 1 disponibles en la MQ y los carga en la MQ nuevamente con su tipo
 * de reciclado correspondiente. 
 **/
void clasificar() {
	while(1) {
		MENSAJE item;
		
		if (msgrcv(mq, &item, SIZE_MENSAJE, 1, IPC_NOWAIT) != -1) { // Si hay items disponibles para clasificar:
			
			if (strcmp(item.nombre, "Vidrio") == 0) {
				item.tipo = VIDRIO;
				strcpy(item.nombre, "Vidrio");
			} else if (strcmp(item.nombre, "Carton") == 0) {
				item.tipo = CARTON;
				strcpy(item.nombre, "Carton");
			} else if (strcmp(item.nombre, "Plastico") == 0) {
				item.tipo = PLASTICO;
				strcpy(item.nombre, "Plastico");
			} else if (strcmp(item.nombre, "Aluminio") == 0) {
				item.tipo = ALUMINIO;
				strcpy(item.nombre, "Aluminio");
			} 
			
			if (msgsnd(mq, &item, SIZE_MENSAJE, IPC_NOWAIT) != -1) {
				printf("%sCLASIFICADOR | Item clasificado = %s (%ld) | Proceso = %d %s \n", yellow(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sCLASIFICADOR | No es posible clasificar -> La MQ se encuentra llena | Proceso = %d %s \n", yellow(), getpid(), reset());
			}
			
		} else { // No hay items recolectados disponibles para clasificar.
			printf("%sCLASIFICADOR | No hay elementos disponibles para clasificar | Proceso = %d %s \n", yellow(), getpid(), reset());
		}
		
		sleep(1); // Se duerme el proceso clasificador actual para una mejor visualizacion.
	}
}

void inicializarColaMensajes() {
	mq = msgget(mq_key, 0666);
	
	if (mq == -1) {
		printf("%sOcurrio un error al obtener la cola de mensajes. %s\n", red(), reset());
		exit(-1);
	}
}


int main(int argc, char ** argv) {
	inicializarColaMensajes();
	clasificar();
	
	return 0;
}
