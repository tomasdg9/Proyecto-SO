#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#define RECOLECTORES 3
#define CLASIFICADORES 2
#define RECICLADORES 4
#define PAQUETE_SIZE 5
#define MIN_TIPO 1
#define MAX_TIPO 4

int p1[2], p2[2], p3[2], p4[2], p5[2];
// p1 -> Pipe utilizado por recolectores-clasificadores.
// p2 -> Pipe utilizado por clasificadores-recicladores para items 'Vidrio'
// p3 -> Pipe utilizado por clasificadores-recicladores para items 'Carton'
// p4 -> Pipe utilizado por clasificadores-recicladores para items 'Plastico'
// p5 -> Pipe utilizado por clasificadores-recicladores para items 'Aluminio'

/*
 * struct utilizado para la representacion de los elementos de basura.
 * Son utilizados como items en las operaciones de Pipes. 
 **/
typedef struct basura {
	int tipo;
	char nombre[10];
} ITEM_BASURA;

/*
 * Carga items aleatorios en el paquete recibido.
 * Estos paquetes son utilizados en la recoleccion. 
 **/
void empaquetarPaqueteBasura(ITEM_BASURA paquete[]) {
	for (int i = 0; i < PAQUETE_SIZE; i ++) {
		int random = rand() % (MAX_TIPO - MIN_TIPO + 1) + MIN_TIPO;
		
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
 * Recoleta un nuevo paquete de items y los carga en el pipe de recoleccion-clasificacion
 * para que puedan ser clasificados. 
 **/
void recolectar() {
	close(p1[0]);
	close(p2[0]); close(p2[1]);
	close(p3[0]); close(p3[1]);
	close(p4[0]); close(p4[1]);
	close(p5[0]); close(p5[1]);

	while(1) {
		ITEM_BASURA paquete[PAQUETE_SIZE];
		empaquetarPaqueteBasura(paquete);
		
		for (int i = 0; i < PAQUETE_SIZE; i ++) {
			ITEM_BASURA item = paquete[i];
			
			if (write(p1[1], &item, sizeof(item))) {
				printf("Item recolectado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("El pipe de recolectados (p1) se encuentra lleno | Proceso = %d", getpid());
			}
		}
		
		sleep(5); // Se duerme el proceso recolector actual para una mejor visualizacion.	
	}

	close(p1[1]);
}

/*
 * Clasifica los items disponibles en el pipe de recoleccion-clasificacion
 * y los carga en los pipes de reciclado correspondientes. 
 **/
void clasificar() {
	close(p1[1]);
	close(p2[0]);
	close(p3[0]);
	close(p4[0]);
	close(p5[0]);

	while(1) {
		ITEM_BASURA item;

		if (read(p1[0], &item, sizeof(item)) != -1) { // Si hay items disponibles para clasificar:
			
			switch(item.tipo) { // Se clasifica y distribuye en el Pipe correspondiente
				case 1:
					if (write(p2[1], &item, sizeof(item)) != -1) {
						printf("Item clasificado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("El pipe de Vidrio (p2) se encuentra lleno.");
					}	
					break;
					
				case 2:
					if (write(p3[1], &item, sizeof(item))) {
						printf("Item clasificado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("El pipe de Carton (p3) se encuentra lleno.");
					}
					break;
					
				case 3:
					if (write(p4[1], &item, sizeof(item)) != -1) {
						printf("Item clasificado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("El pipe de Plastico (p4) se encuentra lleno.");
					}
					break;
					
				case 4:
					if (write(p5[1], &item, sizeof(item)) != -1) {
						printf("Item clasificado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
					} else {
						printf("El pipe de Aluminio (p5) se encuentra lleno.");
					}
					break;
			}
			
		} else { // No hay items recolectados disponibles para clasificar.
			printf("No hay elementos para clasificar | Proceso = %d \n", getpid());
		}
		
		sleep(1); // Se duerme el proceso clasificador actual para una mejor visualizacion.
	}

	close(p1[0]);
	close(p2[1]);
	close(p3[1]);
	close(p4[1]);
	close(p5[1]);
}

/*
 * Recicla los items del pipe de Vidrio y en caso de estar vacio, 
 * colabora con los recicladores de Carton, Plastico y Aluminio.
 * Si los otros recicladores no disponen de items en sus respectivos
 * pipes, se toma mate.
 **/
void reciclar_Vidrio() {
	while(1) {
		ITEM_BASURA item;

		if (read(p2[0], &item, sizeof(item)) != -1) { // Intento reciclar del pipe de Vidrio.
			printf("Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("El pipe de Vidrio (p2) se encuentra vacio -> ");
			
			if (read(p3[0], &item, sizeof(item)) != -1 || read(p4[0], &item, sizeof(item)) != -1 || read(p5[0], &item, sizeof(item)) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items del pipe de Carton y en caso de estar vacio, 
 * colabora con los recicladores de Vidrio, Plastico y Aluminio.
 * Si los otros recicladores no disponen de items en sus respectivos
 * pipes, se toma mate.
 **/
void reciclar_Carton() {
	while(1) {
		ITEM_BASURA item;

		if (read(p3[0], &item, sizeof(item)) != -1) { // Intento reciclar del pipe de Carton.
			printf("Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("El pipe de Carton (p3) se encuentra vacio -> ");
			
			if (read(p2[0], &item, sizeof(item)) != -1 || read(p4[0], &item, sizeof(item)) != -1 || read(p5[0], &item, sizeof(item)) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}

		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items del pipe de Plastico y en caso de estar vacio, 
 * colabora con los recicladores de Vidrio, Carton y Aluminio.
 * Si los otros recicladores no disponen de items en sus respectivos
 * pipes, se toma mate.
 **/
void reciclar_Plastico() {
	while(1) {
		ITEM_BASURA item;

		if (read(p4[0], &item, sizeof(item)) != -1) { // Intento reciclar del pipe de Plastico.
			printf("Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("El pipe de Plastico (p4) se encuentra vacio -> ");
			
			if (read(p2[0], &item, sizeof(item)) != -1 || read(p3[0], &item, sizeof(item)) != -1 || read(p5[0], &item, sizeof(item)) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}

		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items del pipe de Aluminio y en caso de estar vacio, 
 * colabora con los recicladores de Vidrio, Carton y Plastico.
 * Si los otros recicladores no disponen de items en sus respectivos
 * pipes, se toma mate.
 **/
void reciclar_Aluminio() {
	while(1) {
		ITEM_BASURA item;

		if (read(p5[0], &item, sizeof(item)) != -1) { // Intento reciclar del pipe de Aluminio.

			printf("Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());

		} else { // Ayudo a los otros recicladores.
			printf("El pipe de Aluminio (p5) se encuentra vacio -> ");
			
			if (read(p2[0], &item, sizeof(item)) != -1 || read(p3[0], &item, sizeof(item)) != -1 || read(p4[0], &item, sizeof(item)) != -1) {
				printf("AYUDANDO | Item reciclado = %s (%d) | Proceso = %d \n", item.nombre, item.tipo, getpid());
			} else {
				printf("Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d \n", getpid()); 
			}
		
		}

		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items del pipe correspondiente y en caso de que no hayan
 * items disponibles se ayuda a otro reciclador o se toma mate.
 **/
void reciclar(int k) {
	close(p1[0]); close(p1[1]);
	close(p2[1]);
	close(p3[1]);
	close(p4[1]);
	close(p5[1]);
	
	switch(k) {
		case 0: reciclar_Vidrio(); break;
		case 1: reciclar_Carton(); break;
		case 2: reciclar_Plastico(); break;
		case 3: reciclar_Aluminio(); break;
	}

	close(p2[0]);
	close(p3[0]);
	close(p4[0]);
	close(p5[0]);
}

/*
 * Crea e inicializa los pipes como NO bloqueantes.
 * Esto es de gran utilidad para poder trazar el programa cuando se intenta realizar
 * un read() y el pipe esta vacio o cuando se intenta realizar un write() y el pipe
 * esta lleno. 
 **/
void inicializarPipes() {
	pipe(p1);
    pipe(p2);
	pipe(p3);
	pipe(p4);
	pipe(p5);
	
	if(fcntl(p1[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p1[1], F_SETFL, O_NONBLOCK) == -1){
		printf("Ocurrio un problema al convertir el pipe p1 en NO BLOQUEANTE");
		exit(-1);
	}

	if(fcntl(p2[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p2[1], F_SETFL, O_NONBLOCK) == -1){
		printf("Ocurrio un problema al convertir el pipe p2 en NO BLOQUEANTE");
		exit(-1);
	}


	if(fcntl(p3[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p3[1], F_SETFL, O_NONBLOCK) == -1){
		printf("Ocurrio un problema al convertir el pipe p3 en NO BLOQUEANTE");
		exit(-1);
	}


	if(fcntl(p4[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p4[1], F_SETFL, O_NONBLOCK) == -1){
		printf("Ocurrio un problema al convertir el pipe p4 en NO BLOQUEANTE");
		exit(-1);
	}


	if(fcntl(p5[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p5[1], F_SETFL, O_NONBLOCK) == -1){
		printf("Ocurrio un problema al convertir el pipe p5 en NO BLOQUEANTE");
		exit(-1);
	}
}

int main() {
	inicializarPipes();
	
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
