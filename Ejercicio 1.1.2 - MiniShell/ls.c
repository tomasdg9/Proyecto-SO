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
    char * path = malloc(sizeof(char) * PATH_MAX); 
    DIR * mydir;
    struct dirent * myfile;
        
    if (argv[1] == NULL) {
        path = getcwd(NULL, 0); // El directorio actual es el path por defecto.
    } else if(argv[2] != NULL){
        red();
		printf(">> ls(): La cantidad de argumentos ingresada no es valida. \n");
        reset();
        return -1;
    } else {
        path = argv[1];
    }
    
    mydir = opendir(path);
    
    if (mydir != NULL){
        white();
        while((myfile = readdir(mydir)) != NULL){
            if (myfile->d_name[0] == '.') continue;
            else printf("%s \n", myfile->d_name);
        }
        closedir(mydir);
        green();
        printf(">> ls(): Listado de directorio %s realizado exitosamente. \n", path);
        reset();
    } else {
        red();
        printf(">> ls(): Error al listar el contenido del directorio %s. \n", path);
        reset();
        return -1;
    }
    
    free(path);
    
    return 0;
}
