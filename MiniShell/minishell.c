#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>

void leerEntrada(char * instruccion, char ** argv) {
	int i = 0;
	char * token;
	int longitud;
	
	printf("\033[1;33m>> \033[0m");
	fgets(instruccion, sizeof(char)*100, stdin);
	longitud = strlen(instruccion);
	instruccion[-- longitud] = '\0';
	token = strtok(instruccion, " ");
		
	while(token != NULL){
		argv[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
		
	argv[i] = NULL;		
}

int ejecutarComando(char * instruccion, char ** argv) {
	char path[20];
	pid_t child_pid;
		
	if(strcmp(argv[0], "exit") == 0) {
		return 1;
	}
		
	strcpy(path, "./");
	strcat(path, argv[0]);

	argv[0] = path;

	child_pid = fork();
		
	if (child_pid == -1) {
		printf("fork(): Error al crear el proceso hijo. \n");
	} else if (child_pid == 0) {
		
		if(execvp(argv[0], argv) == -1){
			printf("execv(): Error al ejecutar el comando %s. \n", argv[0]);
			fflush(NULL);
		}
		exit(0);
		
	}
	
	wait(NULL);
	
	return 0;
}

int main(){
    char * instruccion = (char*) malloc(sizeof(char)*100);
	char * argv[20];

	for (int i = 0; i < 20; i++){
		argv[i] = NULL;
	}

    while(1){
		leerEntrada(instruccion, argv);
		if (ejecutarComando(instruccion, argv) == 1) break;
    }
    
    free(instruccion);

    return 0;
}
