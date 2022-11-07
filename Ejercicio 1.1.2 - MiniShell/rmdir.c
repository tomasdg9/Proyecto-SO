#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "colores.h"

/*
 * Comando rmdir(): Remover un directorio.
 **/
int main(int argc, char ** argv){
    if (argv[1] == NULL) {
	white();
	printf(">> rmdir(): Debe ingresar el nombre del archivo que desea eliminar. \n");
	reset();
	return -1;
    } else if (argv[2] != NULL){
	red();
	printf(">> rmdir(): Error de ingreso de argumentos. \n");
	reset();
	return -1;
    } else if (rmdir(argv[1]) == -1) {
	red();
	printf(">> rmdir(): Error al remover el directorio %s. \n", argv[1]);
	reset();
	return -1;
    } else {
	green();
	printf(">> rmdir(): Directorio %s removido con éxito. \n", argv[1]);
	reset();
    }
    
    return 0;
}
