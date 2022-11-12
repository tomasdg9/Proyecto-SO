#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "colores.h"

/*
 * Parsea la instruccion ingresada por el usuario y divide los argumentos. 
 */
void leerEntrada(char * instruccion, char ** argumentos) {
	char * token;
	int i = 0;
	
	printf("%s>> %s", yellow(), reset());
	
	fgets(instruccion, sizeof(char) * MAX_INPUT, stdin); // Se lee la instrucción ingresada.
	instruccion[strlen(instruccion) - 1] = '\0'; // Se agrega el fin de instrucción.
	token = strtok(instruccion, " ");
		
	while(token != NULL){
		// Se divide cada uno de los argumentos de la instrucción.
		argumentos[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	
	argumentos[i] = NULL; // Se marca el último argumento como NULL para referencia en los comandos.
}

/*
 * Crea un proceso hijo que ejecuta la instruccion parseada previamente.
 */ 
int ejecutarComando(char ** argumentos) {
	char * path;
	pid_t child_pid;
		
	if(strcmp(argumentos[0], "exit") == 0) {
		return 1;
	}
	
	path = (char *) malloc (sizeof(char) * PATH_MAX);
	
	strcpy(path, "./");
	strcat(path, argumentos[0]);
	argumentos[0] = path;

	child_pid = fork(); // Se crea el proceso hijo.
		
	if (child_pid == -1) {
		printf("%sfork(): Error al crear el proceso hijo. %s\n", red(), reset());
	} else if (child_pid == 0) {
		if(execvp(argumentos[0], argumentos) == -1){ // El proceso hijo ejecuta el comando ingresado.
			printf("%sexecvp(): Error al ejecutar el comando %s en el proceso %i. %s\n", red(), argumentos[0], getpid(), reset());
			fflush(NULL);
		}
		exit(0);	
	}
	
	wait(NULL);
	
	free(path);
	
	return 0;
}

/*
 * minishell: Lee una instruccion por consola y la ejecuta. 
 */
int main(){
	char * instruccion;
	char * argumentos[MAX_INPUT];
	
	instruccion = (char *) malloc(sizeof(char) * MAX_INPUT); 
	
    while(1){
		leerEntrada(instruccion, argumentos);
		if (ejecutarComando(argumentos) == 1) break;
    }
    
    free(instruccion);
    
    return 0;
}
