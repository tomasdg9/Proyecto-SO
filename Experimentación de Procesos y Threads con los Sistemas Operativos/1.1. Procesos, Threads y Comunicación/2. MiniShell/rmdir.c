#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "colores.h"

/*
 * Comando rmdir(): Remover un directorio.
 **/
int main(int argc, char ** argv){
    if (argv[1] == NULL) {
	printf("%s>> rmdir(): Debe ingresar el nombre del archivo que desea eliminar. %s\n", white(), reset());
	return -1;
    } else if (argv[2] != NULL){
	printf("%s>> rmdir(): Error de ingreso de argumentos. %s\n", red(), reset());
	return -1;
    } else if (rmdir(argv[1]) == -1) {
	printf("%s>> rmdir(): Error al remover el directorio %s. %s\n", red(), argv[1], reset());
	return -1;
    } else {
	printf("%s>> rmdir(): Directorio %s removido con éxito. %s\n", green(), argv[1], reset());
    }
    
    return 0;
}
