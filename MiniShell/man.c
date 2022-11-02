#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void man_mkdir() {
	printf("mkdir(): Crea un directorio en la ubicacion de compilacion de la minishell. \n");
	printf("Formato del comando: mkdir [nombre_directorio] \n");
	printf("Observaciones: \n");
	printf("\t -> En caso de que se pueda crear el directorio, se mostrara un mensaje de exito. \n");
	printf("\t -> En caso contrario, se mostrara un mensaje de error. \n");
}

void man_rmdir() {
	printf("mkdir(): Remueve un directorio en la ubicacion de compilacion de la minishell. \n");
	printf("Formato del comando: rmdir [nombre_directorio] \n");
	printf("Observaciones: \n");
	printf("\t -> En caso de que se pueda remover el directorio, se mostrara un mensaje de exito. \n");
	printf("\t -> En caso contrario, se mostrara un mensaje de error. \n");	
}

void man_mkfile() {
	printf("mkfile(): Crea un archivo en la ubicacion de compilacion de la minishell. \n");
	printf("Formato del comando: mkfile [nombre_archivo] \n");
	printf("Observaciones: \n");
	printf("\t -> En caso de que se pueda crear el archivo, se mostrara un mensaje de exito. \n");
	printf("\t -> En caso contrario, se mostrara un mensaje de error. \n");	
}

void man_swfile() {	
	printf("swfile(): Muestra el contenido del archivo ingresado. \n");
	printf("Formato del comando: swfile [path_archivo] \n");
	printf("Observaciones: \n");
	printf("\t -> En caso de exito se muestra el contenido del archivo y un mensaje de exito. \n");
	printf("\t -> En caso contrario, se muestra un mensaje de error. \n");
}

void man_ls() {
	printf("ls(): Lista el contenido del directorio ingresado. \n");
	printf("Formato del comando: ls [path_directorio] \n");
	printf("Observaciones: \n");
	printf("\t -> En caso de exito se muestra el contenido del directorio y un mensaje de exito. \n");
	printf("\t -> En caso contrario, se muestra un mensaje de error. \n");	
}

void man_chmod() {
	printf("chmod(): Cambia los permisos del archivo ingresado. \n");
	printf("Formato del comando: chmod [path_archivo][permisos] \n");
	printf("[permisos] es un numero entero de 3 digitos referenciado a los permisos <Owner><Group><Others>. \n");
	printf("\t 1: Permiso de ejecucion. \n");
	printf("\t 2: Permiso de escritura. \n");
	printf("\t 4: Permiso de lectura. \n");
	printf("\t 7: Permiso de ejecucion + escritura + lectura. \n");
	printf("Observaciones: \n");
	printf("\t -> En caso de exito se cambian los permisos del archivo y se muestra un mensaje de exito. \n");
	printf("\t -> En caso contrario, se muestra un mensaje de error. \n");	
}

int main (int argc, char * argv[]){
	char * instruccion = argv[1];
	
	if (argv[1] == NULL){
		printf("man(): No se ingreso un argumento. La sintaxis es: 'man <comando>' \n");
		return 0;
	}

	if(argv[2] != NULL){
		printf("man(): La cantidad de argumentos ingresada es incorrecta. \n");
		return 0;
	}

	if (strcmp(instruccion, "mkdir") == 0){
		man_mkdir();
	}
	else if (strcmp(instruccion, "rmdir") == 0){
		man_rmdir();
	}
	else if (strcmp(instruccion, "mkfile") == 0){
		man_mkfile();
	}
	else if (strcmp(instruccion, "showfile") == 0){
		man_swfile();
	}
	else if (strcmp(instruccion, "ls") == 0){
		man_ls();
	}
	else if (strcmp(instruccion, "chmod") == 0){
		man_chmod();
	}
	else{
		printf("man(): El comando ingresado no esta implementado en la minishell. \n");
	}

	return 0;
}
