#include <stdlib.h>
#include <stdio.h>
#include "colores.h"

/*
 * Comando mkfile(): Crear un archivo. 
 */
int main(int argc, char ** argv){
    FILE * myfile;
    
    if (argv[1] == NULL) {
        white();
        printf(">> mkfile(): Debe ingresar un nombre para el archivo. \n");
        reset();
        return -1;
    } else if (argv[2] != NULL){
        red();
		printf(">> mkfile(): La cantidad de argumentos ingresada es incorrecta. \n");
        reset();
        return -1;
	} else {
        myfile = fopen(argv[1], "a");
        
        if (myfile != NULL) {
            green();
            printf(">> mkfile(): El archivo %s fue creado exitosamente o ya existe. \n", argv[1]);
            reset();
            fclose(myfile);
        } else {
            red();
            printf(">> mkfile(): Ocurrio un error al crear el archivo %s. \n", argv[1]);
            reset();
            return -1;
        }
    }
    
    return 0;
}
