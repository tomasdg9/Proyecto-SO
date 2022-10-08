#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define RECOLECTORES 3
#define CLASIFICADORES 2
#define RECICLADORES 4
#define PAQUETE_SIZE 5
#define MIN_TIPO 1
#define MAX_TIPO 4

int mq1, mq2, mq3, mq4, mq5;
// mq1 -> MQ utilizada por recolectores-clasificadores.
// mq2 -> MQ utilizada por clasificadores-recicladores para items 'Vidrio'
// mq3 -> MQ utilizada por clasificadores-recicladores para items 'Carton'
// mq4 -> MQ utilizada por clasificadores-recicladores para items 'Plastico'
// mq5 -> MQ utilizada por clasificadores-recicladores para items 'Aluminio'

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
		
		paquete[i].tipo = random;
		
		switch(random) {
			case 1: strcpy(paquete[i].nombre, "Vidrio"); break;
			case 2: strcpy(paquete[i].nombre, "Carton"); break;
			case 3: strcpy(paquete[i].nombre, "Plastico"); break;
			case 4: strcpy(paquete[i].nombre, "Aluminio"); break;
		}
	}
}

/*
 * Recoleta un nuevo paquete de items y los carga en la MQ de recoleccion-clasificacion
 * para que puedan ser clasificados. 
 **/
void recolectar() {
	while(1) {
		MENSAJE paquete[PAQUETE_SIZE];
		empaquetarPaqueteBasura(paquete);
		
		for (int i = 0; i < PAQUETE_SIZE; i ++) {
			MENSAJE item = paquete[i];
			int sizeItem = sizeof(item) - sizeof(long);
			
			if (msgsnd(mq1, &item, sizeItem, IPC_NOWAIT) != -1) {
				printf("Item recolectado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("La MQ de recolectados (mq1) se encuentra llena | Proceso = %d \n", getpid());
			}
		}
		
		sleep(5); // Se duerme el proceso recolector actual para una mejor visualizacion.	
	}
}

/*
 * Clasifica los items disponibles en la MQ de recoleccion-clasificacion
 * y los carga en las MQs de reciclado correspondientes. 
 **/
void clasificar() {
	while(1) {
		MENSAJE item;
		int sizeItem = sizeof(item) - sizeof(long);

		if (msgrcv(mq1, &item, sizeItem, 0, IPC_NOWAIT) != -1) { // Si hay items disponibles para clasificar:
			
			switch(item.tipo) { // Se clasifica y distribuye en la MQ correspondiente.
				case 1:
					if (msgsnd(mq2, &item, sizeItem, IPC_NOWAIT) != -1) {
						printf("Item clasificado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("La MQ de Vidrio (mq2) se encuentra llena.");
					}
					break;
				
				case 2:
					if (msgsnd(mq3, &item, sizeItem, IPC_NOWAIT) != -1) {
						printf("Item clasificado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("La MQ de Carton (mq3) se encuentra llena.");
					}
					break;
				
				case 3:
					if (msgsnd(mq4, &item, sizeItem, IPC_NOWAIT) != -1) {
						printf("Item clasificado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("La MQ de Plastico (mq4) se encuentra llena.");
					}
					break;
				
				case 4:
					if (msgsnd(mq5, &item, sizeItem, IPC_NOWAIT) != -1) {
						printf("Item clasificado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("La MQ de vidrio (mq5) se encuentra llena.");
					}
					break;
			}
			
		} else { // No hay items recolectados disponibles para clasificar.
			printf("No hay elementos para clasificar | Proceso = %d \n", getpid());
		}
		
		sleep(1); // Se duerme el proceso clasificador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de la MQ de Vidrio y en caso de estar vacio, 
 * colabora con los recicladores de Carton, Plastico y Aluminio.
 * Si los otros recicladores no disponen de items en sus respectivas
 * MQs, se toma mate.
 **/
void reciclar_Vidrio() {
	while(1) {
		MENSAJE item;
		int sizeItem = sizeof(item) - sizeof(long);

		if (msgrcv(mq2, &item, sizeItem, 0, IPC_NOWAIT) != -1) { // Intento reciclar del pipe de Vidrio.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("La MQ de Vidrio (mq2) se encuentra vacia -> ");
			
			if (msgrcv(mq3, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq4, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq5, &item, sizeItem, 0, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de la MQ de Carton y en caso de estar vacio, 
 * colabora con los recicladores de Vidrio, Plastico y Aluminio.
 * Si los otros recicladores no disponen de items en sus respectivas
 * MQs, se toma mate.
 **/
void reciclar_Carton() {
	while(1) {
		MENSAJE item;
		int sizeItem = sizeof(item) - sizeof(long);

		if (msgrcv(mq3, &item, sizeItem, 0, IPC_NOWAIT) != -1) { // Intento reciclar del pipe de Vidrio.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("La MQ de Carton (mq3) se encuentra vacia -> ");
			
			if (msgrcv(mq2, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq4, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq5, &item, sizeItem, 0, IPC_NOWAIT)) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de la MQ de Plastico y en caso de estar vacio, 
 * colabora con los recicladores de Vidrio, Carton y Aluminio.
 * Si los otros recicladores no disponen de items en sus respectivas
 * MQs, se toma mate.
 **/
void reciclar_Plastico() {
	while(1) {
		MENSAJE item;
		int sizeItem = sizeof(item) - sizeof(long);

		if (msgrcv(mq4, &item, sizeItem, 0, IPC_NOWAIT) != -1) { // Intento reciclar del pipe de Vidrio.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("La MQ de Plastico (mq4) se encuentra vacia -> ");
			
			if (msgrcv(mq2, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq3, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq4, &item, sizeItem, 0, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de la MQ de Aluminio y en caso de estar vacio, 
 * colabora con los recicladores de Vidrio, Carton y Plastico.
 * Si los otros recicladores no disponen de items en sus respectivas
 * MQs, se toma mate.
 **/
void reciclar_Aluminio() {
	while(1) {
		MENSAJE item;
		int sizeItem = sizeof(item) - sizeof(long);

		if (msgrcv(mq2, &item, sizeItem, 0, IPC_NOWAIT) != -1) { // Intento reciclar del pipe de Vidrio.
			printf("Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("La MQ de Aluminio (mq5) se encuentra vacia -> ");
		
			if (msgrcv(mq3, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq4, &item, sizeItem, 0, IPC_NOWAIT) != -1 || msgrcv(mq5, &item, sizeItem, 0, IPC_NOWAIT) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%ld) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de la MQ correspondiente y en caso de que no hayan
 * items disponibles se ayuda a otro reciclador o se toma mate.
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
void inicializarColasMensajes() {
	key_t k1 = ftok("mq1", 65);
	key_t k2 = ftok("mq2", 65);
	key_t k3 = ftok("mq3", 65);
	key_t k4 = ftok("mq4", 65);
	key_t k5 = ftok("mq5", 65);
	
	mq1 = msgget (k1, 0666 | IPC_CREAT);
	mq2 = msgget (k2, 0666 | IPC_CREAT);
	mq3 = msgget (k3, 0666 | IPC_CREAT);
	mq4 = msgget (k4, 0666 | IPC_CREAT);
	mq5 = msgget (k5, 0666 | IPC_CREAT);
}

int main() {
	inicializarColasMensajes();
	
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

	return 0;
}
