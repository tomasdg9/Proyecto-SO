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
 * Chequea si el contador de los autos del SUR posee un token.
 * Si hay tokens disponibles del SUR, habilita el paso del vehiculo.
 * De lo contrario, bloquea el paso del vehiculo hasta que hayan tokens disponibles.
 */
void coordinarSur() {
	MENSAJE CS;
	msgrcv(mq, &CS, SIZE_MENSAJE, CoordinadorS, 0);
}

/*
 * Si es el ultimo auto del SUR en pasar despues de utilizar todos los tokens disponibles.
 * Es decir, cuando el contador llega a 0, habilita el paso de los autos del NORTE con una 
 * cantidad N de tokens. 
 */
void coordinarNorte() {
	for (int i = 0; i < CONTADOR_PUENTE; i ++) {
		MENSAJE CN = crearMensaje(CoordinadorN);
		msgsnd(mq, &CN, SIZE_MENSAJE, 0);	
	}
}

/*
 * Realiza las acciones de los vehiculos que quieren pasar por el puente
 * y provienen del norte
 **/ 
void vehiculoSur(){
	while(1) {
		MENSAJE S1, S2, S3, S4, S5, S6, S7;
		
		// PROTOCOLO DE ENTRADA
		coordinarSur();
		msgrcv(mq, &S1, SIZE_MENSAJE, MutexS, 0);   
		if (msgrcv(mq, &S2, SIZE_MENSAJE, PasandoS, IPC_NOWAIT) != -1) {
			S2 = crearMensaje(PasandoS);
			msgsnd(mq, &S2, SIZE_MENSAJE, 0);
		} else {
			msgrcv(mq, &S3, SIZE_MENSAJE, Puente, 0);
		}
		S4 = crearMensaje(PasandoS);
		msgsnd(mq, &S4, SIZE_MENSAJE, 0);
		S1 = crearMensaje(MutexS);
		msgsnd(mq, &S1, SIZE_MENSAJE, 0);
			
		// SECCION CRITICA
		printf("%sSUR: Auto del sur pasando el puente. PATENTE: %d %s\n", red(), getpid(), reset());
		sleep(1);
			
		// PROTOCOLO DE SALIDA
		msgrcv(mq, &S5, SIZE_MENSAJE, MutexS, 0);
		msgrcv(mq, &S6, SIZE_MENSAJE, PasandoS, 0);
		if (msgrcv(mq, &S7, SIZE_MENSAJE, PasandoS, IPC_NOWAIT) != -1) {
			S7 = crearMensaje(PasandoS);
			msgsnd(mq, &S7, SIZE_MENSAJE, 0);
		} else {
			S3 = crearMensaje(Puente);
			msgsnd(mq, &S3, SIZE_MENSAJE, 0);
			coordinarNorte();
		}
		S5 = crearMensaje(MutexS);
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
	vehiculoSur();
	
	return 0;
}
