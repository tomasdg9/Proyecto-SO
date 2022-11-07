#include <stdio.h>
#include <stdlib.h>
#include "colores.h"

/*
 * Comando swfile(): Mostrar el contenido de un archivo.
 */ 
int main(int argc, char ** argv){
	FILE * file;
		
	if (argv[1] == NULL) {
		white();
		printf(">> swfile(): Debe ingresar el path del archivo que desea mostrar. \n");
		reset();
		return -1;
	} else if(argv[2] != NULL) {
		red();
		printf(">> swfile(): La cantidad de argumentos ingresados es incorrecta. \n");
		reset();
		return -1;
	} else {
		file = fopen(argv[1], "r");
		
		if (file != NULL){
			white();
			while(!feof(file)) {
				printf("%c", fgetc(file));
			}
			fclose(file);	
			printf("\n");
			green();
			printf(">> swfile(): Se mostro el archivo %s exitosamente. \n", argv[1]);
			reset();
		} else {
			red();
			printf(">> swfile(): Error al abrir el archivo %s \n", argv[1]);
			reset();
			return -1;
		}
	}
	
	return 0;
}
