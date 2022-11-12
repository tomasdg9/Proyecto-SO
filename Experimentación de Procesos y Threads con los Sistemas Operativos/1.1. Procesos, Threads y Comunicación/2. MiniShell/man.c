#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "colores.h"

void man_mkdir() {
	printf("%s", cyan());
	printf("mkdir(): Crea un directorio en la ubicación de compilacion de la minishell. \n");
	printf("Formato del comando: mkdir [nombre_directorio] \n");
	printf("[nombre_directorio] es el nombre del directorio que se crea. \n");
	printf("Valores de retorno: \n");
	printf("\t -> En caso de exito: 0 | Se crea el directorio y se muestra un mensaje de éxito. \n");
	printf("\t -> En caso contrario: -1 | Se muestra un mensaje de error. \n");
	printf("%s", reset());
}

void man_rmdir() {
	printf("%s", cyan());
	printf("rmdir(): Remueve un directorio en la ubicación de compilación de la minishell. \n");
	printf("Formato del comando: rmdir [nombre_directorio] \n");
	printf("[nombre_directorio] es el nombre del directorio que se elimina. \n");
	printf("Valores de retorno: \n");
	printf("\t -> En caso de exito: 0 | Se remueve el directorio y se muestra un mensaje de éxito. \n");
	printf("\t -> En caso contrario: -1 | Se muestra un mensaje de error. \n");	
	printf("%s", reset());
}

void man_mkfile() {
	printf("%s", cyan());
	printf("mkfile(): Crea un archivo en la ubicación de compilación de la minishell. \n");
	printf("Formato del comando: mkfile [nombre_archivo] \n");
	printf("[nombre_arhivo] es el nombre del archivo que se crea. \n");
	printf("Valores de retorno: \n");
	printf("\t -> En caso de exito: 0 | Se crea el archivo y se muestra un mensaje de éxito. \n");
	printf("\t -> En caso contrario: -1 | Se muestra un mensaje de error. \n");	
	printf("%s", reset());
}

void man_swfile() {	
	printf("%s", cyan());
	printf("swfile(): Muestra el contenido del archivo ingresado. \n");
	printf("Formato del comando: swfile [path_archivo] \n");
	printf("[path_archivo] es el path del archivo que se muestra. \n");
	printf("Valores de retorno: \n");
	printf("\t -> En caso de exito: 0 | Se muestra el contenido del archivo y un mensaje de éxito. \n");
	printf("\t -> En caso contrario: -1 | Se muestra un mensaje de error. \n");
	printf("%s", reset());
}

void man_ls() {
	printf("%s", cyan());
	printf("ls(): Lista el contenido del directorio ingresado. \n");
	printf("Formato del comando: ls [path_directorio] \n");
	printf("[path_directorio] es el path del directorio que se lista. \n");
	printf("-> Si no se ingresa [path_directorio], se utiliza el directorio actual. \n");
	printf("Valores de retorno: \n");
	printf("\t -> En caso de exito: 0 | Se muestra el contenido del directorio y un mensaje de éxito. \n");
	printf("\t -> En caso contrario: -1 | Se muestra un mensaje de error. \n");	
	printf("%s", reset());
}

void man_chmod() {
	printf("%s", cyan());
	printf("chmod(): Cambia los permisos del archivo ingresado. \n");
	printf("Formato del comando: chmod [path_archivo][permisos] \n");
	printf("[path_archivo] es el path del archivo. \n");
	printf("[permisos] es un numero de 3 digitos referenciado a los permisos <Owner><Group><Others>. \n");
	printf("\t 0: Sin permisos. \n");
	printf("\t 1: Permiso de ejecución. \n");
	printf("\t 2: Permiso de escritura. \n");
	printf("\t 3: Permiso de escritura y ejecución. \n");
	printf("\t 4: Permiso de lectura. \n");
	printf("\t 5: Permiso de lectura y ejecución. \n");
	printf("\t 6: Permiso de lectura y escritura. \n");
	printf("\t 7: Permiso de lectura, escritura y ejecución. \n");
	printf("Valores de retorno: \n");
	printf("\t -> En caso de exito: 0 | Se cambian los permisos del archivo y se muestra un mensaje de éxito. \n");
	printf("\t -> En caso contrario: -1 | Se muestra un mensaje de error. \n");
	printf("%s", reset());	
}

/*
 * man(): Mostrar una ayuda con los comandos posibles.
 */
int main (int argc, char ** argv){
	char * instruccion = argv[1];
	
	if (argv[1] == NULL){
		printf("%s>> man(): No se ingresaron argumentos. La sintaxis es: 'man <comando>' %s\n", white(), reset());
		return -1;
	} else if (argv[2] != NULL){
		printf("%s>> man(): La cantidad de argumentos ingresada es incorrecta. %s\n", red(), reset());
		return -1;
	} else if (strcmp(instruccion, "mkdir") == 0){
		man_mkdir();
	} else if (strcmp(instruccion, "rmdir") == 0){
		man_rmdir();
	} else if (strcmp(instruccion, "mkfile") == 0){
		man_mkfile();
	} else if (strcmp(instruccion, "ls") == 0){
		man_ls();
	} else if (strcmp(instruccion, "swfile") == 0){
		man_swfile();
	} else if (strcmp(instruccion, "chmod") == 0){
		man_chmod();
	} else{
		printf("%s>> man(): El comando ingresado no esta implementado en la minishell. %s\n", red(), reset());
		return -1;
	}

	return 0;
}
