#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "colores.h"

#define RECOLECTORES 3
#define CLASIFICADORES 2
#define RECICLADORES 4
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
 * Genera un item de basura aleatorio y lo empaqueta para recolección. 
 **/
ITEM_BASURA empaquetarItem() {
	ITEM_BASURA item;
	long random;
	
	random = rand() % (MAX_TIPO - MIN_TIPO + 1) + MIN_TIPO;		
	item.tipo = random;
	
	switch(random) {	
		case 1: strcpy(item.nombre, "Vidrio"); break;			
		case 2: strcpy(item.nombre, "Carton"); break;
		case 3: strcpy(item.nombre, "Plastico"); break;
		case 4: strcpy(item.nombre, "Aluminio"); break;
	}
	
	return item;
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
	
	srand(time(NULL)); // Se utiliza para la generacion de numeros aleatorios.
	
	while(1) {
		ITEM_BASURA item = empaquetarItem();
		
		if (write(p1[1], &item, sizeof(item))) {
			printf("%sRECOLECTOR | Item recolectado = %s (%d) | Proceso = %d %s \n", red(), item.nombre, item.tipo, getpid(), reset());
		} else {
			printf("%sRECOLECTOR | El pipe de items recolectados (p1) se encuentra lleno | Proceso = %d %s \n", red(), getpid(), reset());
		}
		
		sleep(1); // Se duerme el proceso recolector actual para una mejor visualizacion.	
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
						printf("%sCLASIFICADOR | Item clasificado = %s (%d) | Proceso = %d %s \n", yellow(), item.nombre, item.tipo, getpid(), reset());
					} else {
						printf("%sCLASIFICADOR | No es posible clasificar -> El pipe de Vidrio (p2) se encuentra lleno. | Proceso = %d %s \n", yellow(), getpid(), reset());
					}	
					break;
					
				case 2:
					if (write(p3[1], &item, sizeof(item))) {
						printf("%sCLASIFICADOR | Item clasificado = %s (%d) | Proceso = %d %s \n", yellow(), item.nombre, item.tipo, getpid(), reset());
					} else {
						printf("%sCLASIFICADOR | No es posible clasificar -> El pipe de Carton (p3) se encuentra lleno. | Proceso = %d %s \n", yellow(), getpid(), reset());
					}
					break;
					
				case 3:
					if (write(p4[1], &item, sizeof(item)) != -1) {
						printf("%sCLASIFICADOR | Item clasificado = %s (%d) | Proceso = %d %s \n", yellow(), item.nombre, item.tipo, getpid(), reset());
					} else {
						printf("%sCLASIFICADOR | No es posible clasificar -> El pipe de Plastico (p4) se encuentra lleno. | Proceso = %d %s \n", yellow(), getpid(), reset());
					}
					break;
					
				case 4:
					if (write(p5[1], &item, sizeof(item)) != -1) {
						printf("%sCLASIFICADOR | Item clasificado = %s (%d) | Proceso = %d %s \n", yellow(), item.nombre, item.tipo, getpid(), reset());
					} else {
						printf("%sCLASIFICADOR | No es posible clasificar -> El pipe de Aluminio (p5) se encuentra lleno. | Proceso = %d %s \n", yellow(), getpid(), reset());
					}
					break;
			}
			
		} else { // No hay items recolectados disponibles para clasificar.
			printf("%sCLASIFICADOR | No hay elementos para clasificar | Proceso = %d %s \n", yellow(), getpid(), reset());
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
 * Toma mate durante 3 segundos. 
 */
void tomarMate() {
	sleep(3);
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
				
		if (read(p2[0], &item, sizeof(item)) != -1) { // Intenta reciclar del pipe de Vidrio.
			printf("%sRECICLADOR VIDRIO | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.
			if (read(p3[0], &item, sizeof(item)) != -1 || read(p4[0], &item, sizeof(item)) != -1 || read(p5[0], &item, sizeof(item)) != -1) {
				printf("%sRECICLADOR VIDRIO | AYUDANDO (No hay vidrio para reciclar) | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR VIDRIO | Tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
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
		
		if (read(p3[0], &item, sizeof(item)) != -1) { // Intenta reciclar del pipe de Carton.
			printf("%sRECICLADOR CARTON | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.			
			if (read(p2[0], &item, sizeof(item)) != -1 || read(p4[0], &item, sizeof(item)) != -1 || read(p5[0], &item, sizeof(item)) != -1) {
				printf("%sRECICLADOR CARTON | AYUDANDO (No hay carton para reciclar) | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR CARTON | Tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
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

		if (read(p4[0], &item, sizeof(item)) != -1) { // Intenta reciclar del pipe de Plastico.
			printf("%sRECICLADOR PLASTICO | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.
			if (read(p2[0], &item, sizeof(item)) != -1 || read(p3[0], &item, sizeof(item)) != -1 || read(p5[0], &item, sizeof(item)) != -1) {
				printf("%sRECICLADOR PLASTICO | AYUDANDO (No hay plastico para reciclar) | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR PLASTICO | Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
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

		if (read(p5[0], &item, sizeof(item)) != -1) { // Intenta reciclar del pipe de Aluminio.
			printf("%sRECICLADOR ALUMINIO | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
		} else { // Ayuda a los otros recicladores o toma mate.
			if (read(p2[0], &item, sizeof(item)) != -1 || read(p3[0], &item, sizeof(item)) != -1 || read(p4[0], &item, sizeof(item)) != -1) {
				printf("%sRECICLADOR ALUMINIO | AYUDANDO (No hay aluminio para reciclar) | Item reciclado = %s (%d) | Proceso = %d %s \n", green(), item.nombre, item.tipo, getpid(), reset());
			} else {
				printf("%sRECICLADOR ALUMINIO | Estoy tomando mate (Los otros recicladores no requieren ayuda) | Proceso = %d %s \n", green(), getpid(), reset()); 
				tomarMate();
			}
		}
		
		sleep(1); // Se duerme el proceso reciclador actual para una mejor visualizacion.
	}
}

/*
 * Recicla los items de la MQ correspondiente y en caso de que no hayan items disponibles 
 * se ayuda a otro reciclador o se toma mate.
 * Asigna una tarea principal a cada proceso reciclador a partir de la variable k.
 * Cierra los fd no necesarios para las tareas de reciclado.
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
		printf("%sError al convertir el pipe p1 en NO BLOQUEANTE. %s \n", red(), reset());
		exit(-1);
	}

	if(fcntl(p2[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p2[1], F_SETFL, O_NONBLOCK) == -1){
		printf("%sError al convertir el pipe p2 en NO BLOQUEANTE. %s \n", red(), reset());
		exit(-1);
	}


	if(fcntl(p3[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p3[1], F_SETFL, O_NONBLOCK) == -1){
		printf("%sError al convertir el pipe p3 en NO BLOQUEANTE. %s \n", red(), reset());
		exit(-1);
	}


	if(fcntl(p4[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p4[1], F_SETFL, O_NONBLOCK) == -1){
		printf("%sError al convertir el pipe p4 en NO BLOQUEANTE. %s \n", red(), reset());
		exit(-1);
	}


	if(fcntl(p5[0], F_SETFL, O_NONBLOCK) == -1 || fcntl(p5[1], F_SETFL, O_NONBLOCK) == -1){
		printf("%sError al convertir el pipe p5 en NO BLOQUEANTE. %s \n", red(), reset());
		exit(-1);
	}
}

int main() {
	inicializarPipes();
	
	for (int i = 0; i < RECOLECTORES; i ++) { // Se crean los recolectores.
		pid_t p_recolector = fork();

		if (p_recolector == 0) {
			recolectar();
			printf("%s", reset());
			exit(0);
		}
	}

	sleep(1); // Se duerme el proceso padre para que los recolectores comiencen a producir antes de que los clasificadores consuman.

	for (int j = 0; j < CLASIFICADORES; j ++) { // Se crean los clasificadores.
		pid_t p_clasificador = fork();

		if (p_clasificador == 0) {
			clasificar();
			printf("%s", reset());
			exit(0);
		}
	}
	
	sleep(1); // Se duerme el proceso padre para que los clasificadores comiencen a producir antes de que los recicladores consuman.
	
	for (int k = 0; k < RECICLADORES; k ++) { // Se crean los recicladores.
		pid_t p_reciclador = fork();

		if (p_reciclador == 0) {
			reciclar(k);
			printf("%s", reset());
			exit(0);
		}
	}
		
	while(wait(NULL) > 0); // El proceso padre espera a que todos los hijos terminen.
	
	return 0;
}
