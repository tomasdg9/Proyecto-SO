#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/msg.h>
#include <string.h>
#include "colores.h"

#define mq_key 1234
#define MIN_TIPO 1
#define MAX_TIPO 4
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
 * Carga items aleatorios en el paquete recibido.
 * Estos paquetes son utilizados en la recoleccion. 
 **/
MENSAJE empaquetarItem() {
	MENSAJE item;
	long random;
	
	random = rand() % (MAX_TIPO - MIN_TIPO + 1) + MIN_TIPO;
	item.tipo = 1;
	
	switch(random) {	
		case 1: strcpy(item.nombre, "Vidrio"); break;			
		case 2: strcpy(item.nombre, "Carton"); break;
		case 3: strcpy(item.nombre, "Plastico"); break;
		case 4: strcpy(item.nombre, "Aluminio"); break;
	}
	
	return item;
}

/*
 * Genera un item de basura aleatorio y lo empaqueta para recolección. 
 **/
void recolectar() {
	srand(time(NULL)); // Se utiliza para la generacion de numeros aleatorios.
	
	while(1) {
		MENSAJE item = empaquetarItem();
		
		if (msgsnd(mq, &item, SIZE_MENSAJE, IPC_NOWAIT) != -1) {
			printf("%sRECOLECTOR | Item recolectado = %s (%ld) | Proceso = %d %s \n", red(), item.nombre, item.tipo, getpid(), reset());
		} else {
			printf("%sRECOLECTOR | No es posible recolectar -> La MQ se encuentra llena | Proceso = %d %s \n", red(), getpid(), reset());
		}
		
		sleep(1); // Se duerme el proceso recolector actual para una mejor visualizacion.	
	}
}

void inicializarColaMensajes() {
	mq = msgget (mq_key, 0666);
	
	if (mq == -1) {
		printf("%sOcurrio un error al crear la cola de mensajes. %s\n", red(), reset());
		exit(-1);
	}
}

int main(int argc, char ** argv) {
	inicializarColaMensajes();
	recolectar();

	return 0;
}

