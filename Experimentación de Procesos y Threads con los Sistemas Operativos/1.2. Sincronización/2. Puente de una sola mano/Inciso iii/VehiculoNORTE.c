#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include "colores.h"
#include "mensajes.h"

#define mq_key 1234

int mq;

/*
 * Chequea si el contador de los autos del NORTE posee un token.
 * Si hay tokens disponibles del NORTE, habilita el paso del vehiculo.
 * De lo contrario, bloquea el paso del vehiculo hasta que hayan tokens disponibles.
 */
void coordinarNorte() {
	MENSAJE CN;
	msgrcv(mq, &CN, SIZE_MENSAJE, CoordinadorN, 0); 
}

/*
 * Si es el ultimo auto del NORTE en pasar despues de utilizar todos los tokens disponibles.
 * Es decir, cuando el contador llega a 0, habilita el paso de los autos del SUR con una 
 * cantidad N de tokens. 
 */
void coordinarSur() {
	for (int i = 0; i < CONTADOR_PUENTE; i ++) {
		MENSAJE CS = crearMensaje(CoordinadorS);
		msgsnd(mq, &CS, SIZE_MENSAJE, 0);
	}
}

/*
 * Realiza las acciones de los vehiculos provenientes del NORTE al cruzar el puente. 
 **/
void vehiculoNorte() {	
	while(1) {
		MENSAJE S1, S2, S3, S4, S5, S6, S7;
		
		// PROTOCOLO DE ENTRADA
		coordinarNorte(); 
		msgrcv(mq, &S1, SIZE_MENSAJE, MutexN, 0);
		if (msgrcv(mq, &S2, SIZE_MENSAJE, PasandoN, IPC_NOWAIT) != -1) {
			S2 = crearMensaje(PasandoN);
			msgsnd(mq, &S2, SIZE_MENSAJE, 0);
		} else {
			msgrcv(mq, &S3, SIZE_MENSAJE, Puente, 0);
		}
		S4 = crearMensaje(PasandoN);
		msgsnd(mq, &S4, SIZE_MENSAJE, 0);
		S1 = crearMensaje(MutexN);
		msgsnd(mq, &S1, SIZE_MENSAJE, 0);
			
		// SECCION CRITICA
		printf("%sNORTE: Auto del norte pasando el puente. PATENTE: %d %s\n", blue(), getpid(), reset());
		sleep(1);
			
		// PROTOCOLO DE SALIDA
		msgrcv(mq, &S5, SIZE_MENSAJE, MutexN, 0);
		msgrcv(mq, &S6, SIZE_MENSAJE, PasandoN, 0);
		if (msgrcv(mq, &S7, SIZE_MENSAJE, PasandoN, IPC_NOWAIT) != -1) {
			S7 = crearMensaje(PasandoN);
			msgsnd(mq, &S7, SIZE_MENSAJE, 0);
		} else {
			S3 = crearMensaje(Puente);
			msgsnd(mq, &S3, SIZE_MENSAJE, 0);
			coordinarSur();
		}
		S5 = crearMensaje(MutexN);
		msgsnd(mq, &S5, SIZE_MENSAJE, 0);
		
		// SECCION RESTO
	}
}

void inicializarColaMensajes() {
	mq = msgget(mq_key, 0666);
	
	if (mq == -1) {
		printf("%sOcurrio un error al obtener la cola de mensajes. %s\n", red(), reset());
		exit(-1);
	}
}

int main (int argc, char ** argv) {
	inicializarColaMensajes();
	vehiculoNorte();
	
	return 0;
}

