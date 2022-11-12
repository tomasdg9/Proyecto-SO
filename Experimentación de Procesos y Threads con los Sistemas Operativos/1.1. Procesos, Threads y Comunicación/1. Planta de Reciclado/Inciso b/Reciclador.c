#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include "colores.h"

#define mq_key 1234
#define SIZE_MENSAJE sizeof(MENSAJE) - sizeof(long)

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
 * Toma mate durante 3 segundos. 
 */
void tomarMate() {
	sleep(3);
}

/*
 * Recicla los items de Vidrio de la MQ (tipo 2) y en caso de no disponer Vidrio, 
 * colabora con los recicladores de Carton, Plastico y Aluminio.
 * Finalmente, si los otros recicladores no disponen de items de sus respectivos tipos
 * para reciclar, se toma mate.
 **/
void reciclar_Vidrio() {
	while(1) {
		MENSAJE item;
		
		if (msgrcv(mq, &item, SIZE_MENSAJE, 2, IPC_NOWAIT) != -1) { // Intenta reciclar del tipo Vidrio.
			printf("%sRECICLADOR VIDRIO | Item reciclado = %s (%ld) | Proceso = %d %s\n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.
			if (msgrcv(mq, &item, SIZE_MENSAJE, 3, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 4, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 5, IPC_NOWAIT) != -1) {
				printf("%sRECICLADOR VIDRIO | AYUDANDO (No hay vidrio para reciclar) | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR VIDRIO | Tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
			}
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}


/*
 * Recicla los items de Carton de la MQ (tipo 3) y en caso de no disponer Carton, 
 * colabora con los recicladores de Vidrio, Plastico y Aluminio.
 * Finalmente, si los otros recicladores no disponen de items de sus respectivos tipos
 * para reciclar, se toma mate.
 **/
void reciclar_Carton() {
	while(1) {
		MENSAJE item;

		if (msgrcv(mq, &item, SIZE_MENSAJE, 3, IPC_NOWAIT) != -1) { // Intento reciclar del tipo Carton
			printf("%sRECICLADOR CARTON | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.		
			if (msgrcv(mq, &item, SIZE_MENSAJE, 2, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 4, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 5, IPC_NOWAIT) != -1) {
				printf("%sRECICLADOR CARTON | AYUDANDO (No hay carton para reciclar) | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR CARTON | Tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
			}
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de Plastico de la MQ (tipo 3) y en caso de no disponer Plastico, 
 * colabora con los recicladores de Vidrio, Carton y Aluminio.
 * Finalmente, si los otros recicladores no disponen de items de sus respectivos tipos
 * para reciclar, se toma mate.
 **/
void reciclar_Plastico() {
	while(1) {
		MENSAJE item;

		if (msgrcv(mq, &item, SIZE_MENSAJE, 4, IPC_NOWAIT) != -1) { // Intento reciclar del tipo Plastico.
			printf("%sRECICLADOR PLASTICO | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.
			if (msgrcv(mq, &item, SIZE_MENSAJE, 2, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 3, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 5, IPC_NOWAIT) != -1) {
				printf("%sRECICLADOR PLASTICO | AYUDANDO (No hay plastico para reciclar) | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR PLASTICO | Tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
			}
		}
				
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de Aluminio de la MQ (tipo 3) y en caso de no disponer Aluminio, 
 * colabora con los recicladores de Vidrio, Carton y Plastico.
 * Finalmente, si los otros recicladores no disponen de items de sus respectivos tipos
 * para reciclar, se toma mate.
 **/
void reciclar_Aluminio() {
	while(1) {
		MENSAJE item;
		
		if (msgrcv(mq, &item, SIZE_MENSAJE, 5, IPC_NOWAIT) != -1) { // Intento reciclar del tipo Aluminio.
			printf("%sRECICLADOR ALUMINIO | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.	
			if (msgrcv(mq, &item, SIZE_MENSAJE, 2, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 3, IPC_NOWAIT) != -1 || msgrcv(mq, &item, SIZE_MENSAJE, 4, IPC_NOWAIT) != -1) {
				printf("%sRECICLADOR ALUMINIO | AYUDANDO (No hay aluminio para reciclar) | Item reciclado = %s (%ld) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR ALUMINIO | Tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
			}
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

void inicializarColaMensajes() {
	mq = msgget (mq_key, 0666);
	
	if (mq == -1) {
		printf("%sOcurrio un error al crear la cola de mensajes. %s\n", red(), reset());
		exit(-1);
	}
}

/*
 * Recicla los items de la MQ correspondiente y en caso de que no hayan items disponibles 
 * se ayuda a otro reciclador o se toma mate.
 * Asigna una tarea principal a cada proceso reciclador a partir de la variable k.
 **/
void reciclar(int k) {
	switch(k) {
		case 0: reciclar_Vidrio(); break;
		case 1: reciclar_Carton(); break;
		case 2: reciclar_Plastico(); break;
		case 3: reciclar_Aluminio(); break;
	}
}

int main(int argc, char ** argv) {
	int k;
	
	inicializarColaMensajes();
	k = atoi(argv[1]);
	reciclar(k);
	
	return 0;
}
