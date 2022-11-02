#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <mqueue.h>

#define RECOLECTORES 3
#define CLASIFICADORES 2
#define RECICLADORES 4
#define PAQUETE_SIZE 5
#define MIN_TIPO 1
#define MAX_TIPO 4

mqd_t mq;
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
void empaquetarPaqueteBasura(MENSAJE paquete[]) {
	for (int i = 0; i < PAQUETE_SIZE; i ++) {
		long random = rand() % (MAX_TIPO - MIN_TIPO + 1) + MIN_TIPO;
		
		paquete[i].tipo = 1;
		
		switch(random) {
			case 1: strcpy(paquete[i].nombre, "Vidrio"); break;
			case 2: strcpy(paquete[i].nombre, "Carton"); break;
			case 3: strcpy(paquete[i].nombre, "Plastico"); break;
			case 4: strcpy(paquete[i].nombre, "Aluminio"); break;
		}
	}
}

/*
 * Recoleta un nuevo paquete de items y los carga en la MQ como tipo 1.
 * para que puedan ser clasificados. 
 **/
void recolectar() {
	while(1) {
		MENSAJE paquete[PAQUETE_SIZE];
		empaquetarPaqueteBasura(paquete);
		int sizeItem = sizeof(MENSAJE) - sizeof(long);
		
		for (int i = 0; i < PAQUETE_SIZE; i ++) {
			MENSAJE item = paquete[i];
			
			if (msgsnd(mq, &item, sizeItem, IPC_NOWAIT) != -1) {
				printf("Item recolectado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("No es posible recolectar -> La MQ se encuentra llena | Proceso = %d \n", getpid());
			}
		}
		
		sleep(5); // Se duerme el proceso recolector actual para una mejor visualizacion.	
	}
	
	mq_close(mq);
}

/*
 * Clasifica los items de tipo 1 disponibles en la MQ y los carga en la MQ nuevamente con su tipo
 * de reciclado correspondiente. 
 **/
void clasificar() {
	while(1) {
		MENSAJE itemRCV; // Mensaje recibido de recoleccion.
		MENSAJE itemSND; // Mensaje a enviar para reciclado.
		int sizeItem = sizeof(MENSAJE) - sizeof(long);
		
		if (msgrcv(mq, &itemRCV, sizeItem, 1, IPC_NOWAIT) != -1) { // Si hay items disponibles para clasificar:
			
			if (strcmp(itemRCV.nombre, "Vidrio") == 0) {
				itemSND.tipo = 2;
				strcpy(itemSND.nombre, "Vidrio");
			} else if (strcmp(itemRCV.nombre, "Carton") == 0) {
				itemSND.tipo = 3;
				strcpy(itemSND.nombre, "Carton");
			} else if (strcmp(itemRCV.nombre, "Plastico") == 0) {
				itemSND.tipo = 4;
				strcpy(itemSND.nombre, "Plastico");
			} else if (strcmp(itemRCV.nombre, "Aluminio") == 0) {
				itemSND.tipo = 5;
				strcpy(itemSND.nombre, "Aluminio");
			} 
			
			if (msgsnd(mq, &itemSND, sizeItem, IPC_NOWAIT) != -1) {
				printf("Item clasificado = %s (%ld) | Proceso = %d \n", itemSND.nombre, itemSND.tipo, getpid());
			} else {
				printf("No es posible clasificar -> La MQ se encuentra llena | Proceso = %d \n", getpid());
			}
			
		} else { // No hay items recolectados disponibles para clasificar.
			printf("No hay elementos disponibles para clasificar | Proceso = %d \n", getpid());
		}
		
		sleep(1); // Se duerme el proceso clasificador actual para una mejor visualizacion.
	}
	
	mq_close(mq);
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
		int sizeItem = sizeof(MENSAJE) - sizeof(long);	
		
		if (msgrcv(mq, &item, sizeItem, 2, IPC_NOWAIT) != -1) { // Intenta reciclar del tipo Vidrio.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayuda a los otros recicladores o toma mate.
			printf("No hay items de Vidrio disponibles para reciclar -> ");
			
			if (msgrcv(mq, &item, sizeItem, 3, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 4, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 5, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
				sleep(3);
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
	
	mq_close(mq);
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
		int sizeItem = sizeof(MENSAJE) - sizeof(long);

		if (msgrcv(mq, &item, sizeItem, 3, IPC_NOWAIT) != -1) { // Intento reciclar del tipo Carton
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayuda a los otros recicladores o toma mate.		
			printf("No hay items de Cartom disponibles para reciclar -> ");
			
			if (msgrcv(mq, &item, sizeItem, 2, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 4, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 5, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
				sleep(3);
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
	
	mq_close(mq);
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
		int sizeItem = sizeof(MENSAJE) - sizeof(long);

		if (msgrcv(mq, &item, sizeItem, 4, IPC_NOWAIT) != -1) { // Intento reciclar del tipo Plastico.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayuda a los otros recicladores o toma mate.
			printf("No hay items de Plastico disponibles para reciclar -> ");
			
			if (msgrcv(mq, &item, sizeItem, 2, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 3, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 5, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
				sleep(3);
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
	
	mq_close(mq);
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
		int sizeItem = sizeof(MENSAJE) - sizeof(long);
		
		if (msgrcv(mq, &item, sizeItem, 5, IPC_NOWAIT) != -1) { // Intento reciclar del tipo Aluminio.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayuda a los otros recicladores o toma mate.
			printf("No hay items de Aluminio disponibles para reciclar -> ");
			
			if (msgrcv(mq, &item, sizeItem, 2, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 3, IPC_NOWAIT) != -1 || msgrcv(mq, &item, sizeItem, 4, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
				sleep(3);
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
	
	mq_close(mq);
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

/*
 * Crea e inicializa las colas de mensajes.
 * Se utiliza para las MQ en msgsnd() y msgrcv() IPC_NOWAIT para evitar que se bloqueen 
 * los procesos al intentar enviar un mensaje en una MQ llena o intentar leer de una MQ
 * vacia y poder asi trazar mejor el programa.
 **/
void inicializarColaMensajes() {
	// key -> Se puede generar con ftok, pero no es necesario.
	// key_t k = ftok("path", 65);
	mq = msgget (1234, IPC_CREAT | 0666);
}

int main() {
	inicializarColaMensajes();
	
	srand(time(NULL)); // Se utiliza para la generacion de numeros aleatorios.

	for (int i = 0; i < RECOLECTORES; i ++) { // Se crean los recolectores.
		pid_t p_recolector = fork();

		if (p_recolector == 0) {
			recolectar();
			exit(0);
		}
	}

	sleep(1); // Se duerme el proceso padre para que los recolectores comiencen a producir antes de que los clasificadores consuman.

	for (int j = 0; j < CLASIFICADORES; j ++) { // Se crean los clasificadores.
		pid_t p_clasificador = fork();

		if (p_clasificador == 0) {
			clasificar();
			exit(0);
		}
	}
	
	
	sleep(1); // Se duerme el proceso padre para que los clasificadores comiencen a producir antes de que los recicladores consuman.
	
	for (int k = 0; k < RECICLADORES; k ++) { // Se crean los recicladores.
		pid_t p_reciclador = fork();

		if (p_reciclador == 0) {
			reciclar(k);
			exit(0);
		}
	}
	
	while(wait(NULL) > 0); // El proceso padre espera a que todos los hijos terminen.
	
	msgctl(mq, IPC_RMID, NULL);

	return 0;
}
