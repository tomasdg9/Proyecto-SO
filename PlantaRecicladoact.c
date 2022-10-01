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

int generarItemBasura() {
	return rand() % 4;
}

void recolectar(int p1[]) {
	close(p1[0]);

	while(1) {
		int item = generarItemBasura();
		write(p1[1], &item, sizeof(int));
		printf("Item recolectado = %d | Proceso = %d \n", item, getpid());
		sleep(1); //Para visualizar mejor los procesos
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

		if (read(p1[0], &item, sizeof(int)) != -1) {//Para que pase esto el pipe tiene que ser bloqueante (Ya esta hecho en el main)
			switch(item) {
				case 0:
					write(p2[1], &item, sizeof(int));
					printf("Item clasificado = %d (Vidrio) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 1:
					write(p3[1], &item, sizeof(int));
					printf("Item clasificado = %d (Carton) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 2:
					write(p4[1], &item, sizeof(int));
					printf("Item clasificado = %d (Plastico) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 3:
					write(p5[1], &item, sizeof(int));
					printf("Item clasificado = %d (Aluminio) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
			}
		} else {
			printf("No hay elementos para clasificar | Proceso = %d \n", getpid());
			
			sleep(1); //Para visualizar mejor los procesos
		}
	}

	close(p1[0]);
	close(p2[1]);
	close(p3[1]);
	close(p4[1]);
	close(p5[1]);
}

void reciclar(int pAct[], int p1[],int p2[],int p3[],int p4[]) {
	int ayuda;
	close(pAct[1]); //pAct para conocer el pipe del reciclador actual
	close(p1[1]);
	close(p2[1]);
	close(p3[1]);
	close(p4[1]);

	while(1){
		int item;

		if (read(pAct[0], &item, sizeof(item)) != -1) {

			switch(item) {
				case 0:
					printf("Item reciclado = %d (Vidrio) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 1:
					printf("Item reciclado = %d (Carton) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 2:
					printf("Item reciclado = %d (Plastico) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 3:
					printf("Item reciclado = %d (Aluminio) | Proceso = %d \n", item, getpid());
					sleep(1); //Para visualizar mejor los procesos
					break;
			}

		} else {
			printf("No hay elementos para reciclar en el pipe actual | Proceso = %d \n", getpid());
			ayuda = rand() % 4;
			if(ayuda != item){//si es distinto del item actual (significa que quiere ayudar a otro reciclador)
				switch(ayuda) {
				case 0:
					if(read(p1[0], &item, sizeof(item)) != -1) {//si el pipe vidrio tiene algun elemento disponible para leer
						printf("Item reciclado (a traves de ayuda) = %d (Vidrio) | Proceso = %d \n", ayuda, getpid());
					}else{
						printf("Estoy tomando mate | Proceso = %d \n", getpid());
					}	
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 1:
					if(read(p2[0], &item, sizeof(item)) != -1) {//si el pipe carton tiene algun elemento disponible para leer
						printf("Item reciclado (a traves de ayuda) = %d (Carton) | Proceso = %d \n", ayuda , getpid());
					}else{
						printf("Estoy tomando mate | Proceso = %d \n", getpid());
					}	
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 2:
					if(read(p3[0], &item, sizeof(item)) != -1) {//si el pipe plastico tiene algun elemento disponible para leer
						printf("Item reciclado (a traves de ayuda) = %d (Plastico) | Proceso = %d \n", ayuda, getpid());
					}else{
						printf("Estoy tomando mate | Proceso = %d \n", getpid());
					}	
					sleep(1); //Para visualizar mejor los procesos
					break;
				case 3:
					if(read(p4[0], &item, sizeof(item)) != -1) {//si el pipe aluminio tiene algun elemento disponible para leer
						printf("Item reciclado (a traves de ayuda) = %d (Aluminio) | Proceso = %d \n", ayuda, getpid());
					}else{
						printf("Estoy tomando mate | Proceso = %d \n", getpid());
					}
					sleep(1); //Para visualizar mejor los procesos
					break;
			}
			}else{
				printf("Estoy tomando mate | Proceso = %d \n", getpid());
			}
		}
	}

	close(pAct[0]); //pAct para conocer el pipe del reciclador actual
	close(p1[0]);
	close(p2[0]);
	close(p3[0]);
	close(p4[0]);
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

    //Hago todos los pipes bloquentes, esto hace que si queremos leer
    //de un pipe sin elementos, no se queda esperando (bloqueado) esperando
    //leer un item (esto ayuda para hacer la parte de los recicladores cuando toman mate, etc.)
    //Al leer el pipe actuaria como una especie de "try wait"
    if(fcntl(p1[0], F_SETFL, O_NONBLOCK) < 0){
        printf("El pipe p1 no pudo hacerse bloqueante");
        exit(-1);//Larga error si no se puede hacer bloqueante
    }
    if(fcntl(pi[0][0], F_SETFL, O_NONBLOCK) < 0){
        printf("El pipe pi[0] no pudo hacerse bloqueante");
        exit(-1);//Larga error si no se puede hacer bloqueante
    }
    if(fcntl(pi[1][0], F_SETFL, O_NONBLOCK) < 0){
        printf("El pipe pi[1] no pudo hacerse bloqueante");
        exit(-1);//Larga error si no se puede hacer bloqueante
    }
    if(fcntl(pi[2][0], F_SETFL, O_NONBLOCK) < 0){
        printf("El pipe pi[2] no pudo hacerse bloqueante");
        exit(-1);//Larga error si no se puede hacer bloqueante
    }
    if(fcntl(pi[3][0], F_SETFL, O_NONBLOCK) < 0){
        printf("El pipe pi[3] no pudo hacerse bloqueante");
        exit(-1);//Larga error si no se puede hacer bloqueante
    }


	for (int i = 0; i < RECOLECTORES; i ++) {
		pid_t p_recolector = fork();

		if (p_recolector == 0) {
			recolectar(p1);
			exit(0);
		}
	}

	sleep(1); //Dormir el proceso padre en este caso, da tiempo a que los recolectores 
	//comiencen a recolectar antes de que los clasificadores quieran clasificar, esto tambien depende del planificador de tareas
	//pero la idea del sleep es esa

	for (int j = 0; j < CLASIFICADORES; j ++) {
		pid_t p_clasificador = fork();

		if (p_clasificador == 0) {
			clasificar(p1, pi[0], pi[1], pi[2], pi[3]);
			exit(0);
		}
	}
	
	sleep(1);//Dormir el proceso padre en este caso, da tiempo a que los clasificadores 
	//comiencen a clasificar antes de que los recicladores quieran recolectar, esto tambien depende del planificador de tareas
	//pero la idea del sleep es esa

	for (int k = 0; k < RECICLADORES; k ++) {
		pid_t p_reciclador = fork();

		if (p_reciclador == 0) {
			reciclar(pi[k], pi[0], pi[1], pi[2], pi[3]); //Cambie esto ya que cada reciclador necesita conocer los 4 pipes en caso de que decida ayudar
			exit(0);
		}
	}

	while(wait(NULL) > 0); // El proceso padre espera a que todos los hijos terminen.

	return 1;
}

