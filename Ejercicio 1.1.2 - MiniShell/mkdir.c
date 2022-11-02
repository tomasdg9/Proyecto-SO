#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

/*
 * Comando mkdir(): Crear un directorio. 
 */
int main(int argc, char * argv[]){
    if(argv[2] != NULL){
	printf("mkdir(): Error de ingreso de argumentos. \n");
	return 0;
    }

    if(mkdir(argv[1], 0666) == 0) {
	printf("\033[1;32m>> mkdir(): Directorio %s creado con éxito. \033[0m \n", argv[1]);
    }
    else {
	printf("\033[1;31m>> mkdir(): Error al crear el directorio %s. \033[0m \n", argv[1]);
    }
    
    return 0;
}
