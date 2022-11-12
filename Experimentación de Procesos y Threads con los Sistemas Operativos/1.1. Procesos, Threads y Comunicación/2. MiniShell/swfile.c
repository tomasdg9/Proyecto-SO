#include <stdio.h>
#include <stdlib.h>
#include "colores.h"

/*
 * Comando swfile(): Mostrar el contenido de un archivo.
 */ 
int main(int argc, char ** argv){
	FILE * file;
		
	if (argv[1] == NULL) {
		printf("%s>> swfile(): Debe ingresar el path del archivo que desea mostrar. %s\n", white(), reset());
		return -1;
	} else if(argv[2] != NULL) {
		printf("%s>> swfile(): La cantidad de argumentos ingresados es incorrecta. %s\n", red(), reset());
		return -1;
	} else {
		file = fopen(argv[1], "r");
		
		if (file != NULL){
			printf("%s", white());
			while(!feof(file)) {
				printf("%c", fgetc(file));
			}
			fclose(file);	
			printf("%s\n", reset());
			printf("%s>> swfile(): Se mostro el archivo %s exitosamente. %s\n", green(), argv[1], reset());
		} else {
			printf("%s>> swfile(): Error al abrir el archivo %s %s\n", red(), argv[1], reset());
			return -1;
		}
	}
	
	return 0;
}
