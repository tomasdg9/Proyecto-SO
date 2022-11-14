#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include "colores.h"

/*
 * Comando ls(): Listar el contenido de un directorio. 
 */
int main(int argc, char ** argv){
    char * path;
    DIR * mydir;
    struct dirent * myfile;
        
    if (argv[1] == NULL) {
        path = getcwd(NULL, 0); // El directorio actual es el path por defecto.
    } else if(argv[2] != NULL){
		printf("%s>> ls(): La cantidad de argumentos ingresada no es valida. %s\n", red(), reset());
        return -1;
    } else {
        path = argv[1];
    }
    
    mydir = opendir(path);
    
    if (mydir != NULL){
        printf("%s", white());
        while((myfile = readdir(mydir)) != NULL){
            if (myfile->d_name[0] == '.') continue; // Si es un archivo oculto se saltea.
            else printf("%s \n", myfile->d_name);
        }
        closedir(mydir);
        printf("%s", reset());
        printf("%s>> ls(): Listado de directorio %s realizado exitosamente. %s\n", green(), path, reset());
    } else {
        printf("%s>> ls(): Error al listar el contenido del directorio %s. %s\n", red(), path, reset());
        return -1;
    }
    
    return 0;
}
