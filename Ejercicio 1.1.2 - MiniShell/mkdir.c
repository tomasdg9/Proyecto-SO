#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include "colores.h"

/*
 * Comando mkdir(): Crear un directorio. 
 */
int main(int argc, char ** argv){
    if (argv[1] == NULL) {
	white();
	printf(">> mkdir(): Debe ingresar el nombre del archivo que desea crear. \n");
	reset();
	return -1;
    } else if (argv[2] != NULL){
	red();
	printf(">> mkdir(): Error de ingreso de argumentos. \n");
	reset();
	return -1;
    } else if (mkdir(argv[1], 0666) == -1) {
	red();
	printf(">> mkdir(): Error al crear el directorio %s. \n", argv[1]);
	reset();
	return -1;
    } else {
	green();
	printf(">> mkdir(): Directorio %s creado con éxito. \n", argv[1]);
	reset();
    }
    
    return 0;
}
