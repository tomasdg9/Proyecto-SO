#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define RECOLECTORES 3
#define CLASIFICADORES 2
#define RECICLADORES 4

int generarItemBasura() {
	return rand() % 4;
}

void recolectar(int p1[]) {
	close(p1[0]);
	
	while(1) {
		int item = generarItemBasura();
		write(p1[1], &item, sizeof(int));
		printf("Item recolectado = %d | Proceso = %d \n", item, getpid());
	}
	
	close(p1[1]);
}

void clasificar(int p1[], int p2[], int p3[], int p4[], int p5[]) {
	close(p1[1]);
	close(p2[0]);
	close(p3[0]);
	close(p4[0]);
	close(p5[0]);
	
	while(1) {
		int item;
		
		if (read(p1[0], &item, sizeof(int)) != -1) {
			switch(item) {
				case 0:
					write(p2[1], &item, sizeof(int));
					printf("Item clasificado = %d (Vidrio) | Proceso = %d \n", item, getpid());
					break;
				case 1:
					write(p3[1], &item, sizeof(int));
					printf("Item clasificado = %d (Carton) | Proceso = %d \n", item, getpid());
					break;
				case 2:
					write(p4[1], &item, sizeof(int));
					printf("Item clasificado = %d (Plastico) | Proceso = %d \n", item, getpid());
					break;
				case 3:
					write(p5[1], &item, sizeof(int));
					printf("Item clasificado = %d (Aluminio) | Proceso = %d \n", item, getpid());
					break;
			}
		} else {
			printf("No hay elementos para clasificar | Proceso = %d \n", getpid());
		}
	}
	
	close(p1[0]);
	close(p2[1]);
	close(p3[1]);
	close(p4[1]);
	close(p5[1]);
}

void reciclar(int pi[]) {
	close(pi[1]);
	
	while(1){
		int item;
		
		if (read(pi[0], &item, sizeof(item)) != -1) {
		
			switch(item) {
				case 0:
					printf("Item reciclado = %d (Vidrio) | Proceso = %d \n", item, getpid());
					break;
				case 1:
					printf("Item reciclado = %d (Carton) | Proceso = %d \n", item, getpid());
					break;
				case 2:
					printf("Item reciclado = %d (Plastico) | Proceso = %d \n", item, getpid());
					break;
				case 3:
					printf("Item reciclado = %d (Aluminio) | Proceso = %d \n", item, getpid());
					break;
			}
		
		} else {
			printf("No hay elementos para reciclar | Proceso = %d \n", getpid());
		}
	}
	
	close(pi[0]);
}

int main() {
	int p1[2]; // Pipe compartido por recolectores y clasificadores.
	int pi[RECICLADORES][2]; // Pipes compartidos por clasificadores y recicladores. (Cada reciclador tiene su propio pipe)
	
	srand(time(NULL));
	
	pipe(p1);
	pipe(pi[0]); // Pipe -> Vidrio
	pipe(pi[1]); // Pipe -> Carton
	pipe(pi[2]); // Pipe -> Plastico
	pipe(pi[3]); // Pipe -> Aluminio
	
	for (int i = 0; i < RECOLECTORES; i ++) {
		pid_t p_recolector = fork();
		
		if (p_recolector == 0) {
			recolectar(p1);
			exit(0);
		}
	}
	
	for (int j = 0; j < CLASIFICADORES; j ++) {
		pid_t p_clasificador = fork();
		
		if (p_clasificador == 0) {
			clasificar(p1, pi[0], pi[1], pi[2], pi[3]);
			exit(0);
		}
	}
	
	for (int k = 0; k < RECICLADORES; k ++) {
		pid_t p_reciclador = fork(); 
		
		if (p_reciclador == 0) {
			reciclar(pi[k]);
			exit(0);
		}
	} 
	
	while(wait(NULL) > 0); // El proceso padre espera a que todos los hijos terminen.
 	
	return 1;
}

/*
int main {
	srand(time(NULL));
	
	for (int i = 0; i < 15; i ++) {
		printf("Num[%d] = %d \n", i, generarItemBasura());
	}
	
	return 1;
}
*/
