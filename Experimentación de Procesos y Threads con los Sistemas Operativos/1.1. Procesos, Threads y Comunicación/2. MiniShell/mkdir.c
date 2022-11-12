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
	printf("%s>> mkdir(): Debe ingresar el nombre del archivo que desea crear. %s\n", white(), reset());
	return -1;
    } else if (argv[2] != NULL){
	printf("%s>> mkdir(): Error de ingreso de argumentos. %s\n", red(), reset());
	return -1;
    } else if (mkdir(argv[1], 0777) == -1) {
	printf("%s>> mkdir(): Error al crear el directorio %s. %s\n", red(), argv[1], reset());
	return -1;
    } else {
	printf("%s>> mkdir(): Directorio %s creado con éxito. %s\n", green(), argv[1], reset());
    }
    
    return 0;
}
