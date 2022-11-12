#include <stdlib.h>
#include <stdio.h>
#include "colores.h"

/*
 * Comando mkfile(): Crear un archivo. 
 */
int main(int argc, char ** argv){
    FILE * myfile;
    
    if (argv[1] == NULL) {
        printf("%s>> mkfile(): Debe ingresar un nombre para el archivo. %s\n", white(), reset());
        return -1;
    } else if (argv[2] != NULL){
		printf("%s>> mkfile(): La cantidad de argumentos ingresada es incorrecta. %s\n", red(), reset());
        return -1;
	} else {
        myfile = fopen(argv[1], "a");
        
        if (myfile != NULL) {
            printf("%s>> mkfile(): El archivo %s fue creado exitosamente o ya existe. %s\n", green(), argv[1], reset());
            fclose(myfile);
        } else {
            printf("%s>> mkfile(): Ocurrio un error al crear el archivo %s. %s\n", red(), argv[1], reset());
            return -1;
        }
    }
    
    return 0;
}
