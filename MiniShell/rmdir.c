#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Comando rmdir(): Remover un directorio.
 **/
int main(int i, char * argv[]){
    if(argv[2] != NULL){
	printf("rmdir(): Error de ingreso de argumentos. \n");
	return 0;
    }

    if(rmdir(argv[1]) == 0) {
	printf("\033[1;32m>> rmdir(): Directorio %s removido con éxito. \033[0m \n", argv[1]);
    }
    else {
	printf("\033[1;31m>> rmdir(): Error al remover el directorio %s. \033[0m \n", argv[1]);
    }
    
    return 0;
}
