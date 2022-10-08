#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pA[2], pB[2], pC[2], pDE[2], pF[2];

/*
 * Muestra la letra A de la secuencia por pantalla. 
 **/
void * A() {
	int signal;
	
	close(pA[1]);
	close(pB[0]);
	close(pC[0]);
	close(pDE[0]); close(pDE[1]);
	close(pF[0]); close(pF[1]);
	
	while(1) {
		read(pA[0], &signal, sizeof(signal));
		printf("A");
		fflush(stdout);
		write(pB[1], &signal, sizeof(signal));
		write(pC[1], &signal, sizeof(signal));
		sleep(1);
	}
}

/*
 * Muestra la letra B de la secuencia por pantalla. 
 **/
void * B() {
	int signal;
	
	close(pA[0]);
	close(pB[1]);
	close(pC[0]); close(pC[1]);
	close(pDE[0]);
	close(pF[0]); close(pF[1]);
	
	while(1) {
		read(pB[0], &signal, sizeof(signal));
		read(pB[0], &signal, sizeof(signal));
		printf("B");
		fflush(stdout);
		write(pDE[1], &signal, sizeof(signal));
		write(pA[1], &signal, sizeof(signal));
		sleep(1);
	}
}

/*
 * Muestra la letra C de la secuencia por pantalla. 
 **/
void * C() {
	int signal;
	
	close(pA[0]); close(pA[1]);
	close(pB[0]); close(pB[1]);
	close(pC[1]);
	close(pDE[0]);
	close(pF[0]); close(pF[1]);
	
	while(1) {
		read(pC[0], &signal, sizeof(signal));
		read(pC[0], &signal, sizeof(signal));
		printf("C");
		fflush(stdout);
		write(pDE[1], &signal, sizeof(signal));
		sleep(1);
	}
}

/*
 * Muestra la letra D de la secuencia por pantalla. 
 **/
void * D() {
	int signal;
	
	close(pA[0]); close(pA[1]);
	close(pB[0]); close(pB[1]);
	close(pC[0]); close(pC[1]);
	close(pDE[1]);
	close(pF[0]);
	
	while (1) {
		read(pDE[0], &signal, sizeof(signal));
		printf("D");
		fflush(stdout);
		write(pF[1], &signal, sizeof(signal));
		sleep(1);
	}
}

/*
 * Muestra la letra E de la secuencia por pantalla. 
 **/
void * E() {
	int signal;
	
	close(pA[0]); close(pA[1]);
	close(pB[0]); close(pB[1]);
	close(pC[0]); close(pC[1]);
	close(pDE[1]);
	close(pF[0]);
	
	while (1) {
		read(pDE[0], &signal, sizeof(signal));
		printf("E");
		fflush(stdout);
		write(pF[1], &signal, sizeof(signal));
		sleep(1);
	}
}

/*
 * Muestra la letra F de la secuencia por pantalla. 
 **/
void * F() {
	int signal;
	
	close(pA[0]);
	close(pB[0]); close(pB[1]);
	close(pC[0]); close(pC[1]);
	close(pDE[0]); close(pDE[1]);
	close(pF[1]);
	
	while(1) {
		read(pF[0], &signal, sizeof(signal));
		read(pF[0], &signal, sizeof(signal));
		printf("F");
		fflush(stdout);
		write(pA[1], &signal, sizeof(signal));
		sleep(1);
	} 
}

/*
 * Inicializa los pipes, escribiendo la cantidad de signals necesarias para la sincronizacion. 
 **/
void inicializarPipes() {
	int signal = 1;
	
	pipe(pA);
	pipe(pB);
	pipe(pC);
	pipe(pDE);
	pipe(pF);
	
	write(pA[1], &signal, sizeof(signal));
	write(pB[1], &signal, sizeof(signal));
}

int main () {
	int childPID;
	inicializarPipes();
	
	for (int i = 0; i < 6; i ++) {
		childPID = fork();
		
		if (childPID == 0) {
			switch(i) {
				case 0: A(); break;
				case 1: B(); break;
				case 2: C(); break;
				case 3: D(); break;
				case 4: E(); break;
				case 5: F(); break;
			}
			
			exit(0);
		}
	}
	
	while(wait(NULL) > 0);
	
	return 0;
}
