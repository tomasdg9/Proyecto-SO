#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "colores.h"

/*
 * Comando chmod(): Modificar los permisos de un archivo. Los permisos son de lectura, escritura y ejecucion.
 */
int main(int argc, char ** argv){
	char * path;
	mode_t modo;
	
	if(argv[1] == NULL) {
		white();
		printf(">> chmod(): Debe ingresar el path del archivo. \n");
		reset();
		return -1;
	} else if(argv[2] == NULL) {
		white();
		printf(">> chmod(): Debe ingresar un argumento entero para identificar los permisos del archivo. \n");
		reset();
		return -1;
	} else if(argv[3] != NULL){
		red();
		printf(">> chmod(): La cantidad de argumentos ingresados es incorrecta. \n");
		reset();
		return -1;
	} else if (atoi(argv[2]) < 0 || atoi(argv[2]) > 777) {
		red();
		printf(">> chmod(): Los permisos ingresados no se encuentran dentro del rango posible. \n");
		reset();
		return -1;
	} else { 
		path = argv[1];
		modo = strtol(argv[2], 0, 8); // Se obtiene el modo en base octal.
		
		if(chmod(path, modo) == 0){
			green();
			printf(">> chmod(): Se cambiaron exitosamente los permisos del archivo %s. \n", path);
			reset();
		} else {
			red();
			printf(">> chmod(): Ocurrio un error al cambiar los permisos del archivo %s. \n", path);
			reset();
			return -1;
		}
	}
	
	return 0;
}
