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
		printf("%s>> chmod(): Debe ingresar el path del archivo. %s\n", white(), reset());
		return -1;
	} else if(argv[2] == NULL) {
		printf("%s>> chmod(): Debe ingresar un argumento entero para identificar los permisos del archivo. %s\n", white(), reset());
		return -1;
	} else if(argv[3] != NULL){
		printf("%s>> chmod(): La cantidad de argumentos ingresados es incorrecta. %s\n", red(), reset());
		return -1;
	} else if(strlen(argv[2]) != 3) {
		printf("%s>> chmod(): Los permisos ingresados no son validos. %s\n", red(), reset());
		return -1;
	} else { 
		path = argv[1];
		modo = strtol(argv[2], 0, 8);
		
		if(chmod(path, modo) == 0){
			printf("%s>> chmod(): Se cambiaron exitosamente los permisos del archivo %s. %s\n", green(), path, reset());
		} else {
			printf("%s>> chmod(): Ocurrio un error al cambiar los permisos del archivo %s. %s\n", red(), path, reset());
			return -1;
		}
	}
	
	return 0;
}
