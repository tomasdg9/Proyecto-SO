# makefile para compilar la minishell

COMANDOS = mkdir rmdir mkfile ls swfile man chmod

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: minishell $(COMANDOS)

# Compilacion del archivo principal.
minishell:
	gcc minishell.c -o minishell
	
# Compilacion del comando: Crear un directorio.
mkdir:
	gcc mkdir.c -o mkdir
	
# Compilacion del comando: Eliminar un directorio.
rmdir:
	gcc rmdir.c -o rmdir

# Compilacion del comando: Crear un archivo.
mkfile:
	gcc mkfile.c -o mkfile
	
# Compilacion del comando: Listar el contenido de un directorio.
ls:
	gcc ls.c -o ls
	
# Compilacion del comando: Mostrar el contenido de un archivo.
swfile:
	gcc swfile.c -o swfile

# Compilacion del comando: Mostrar una ayuda con los comandos posibles.
man:
	gcc man.c -o man
	
# Compilacion del comando: Modificar los permisos de un archivo.
chmod:
	gcc chmod.c -o chmod
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f minishell $(COMANDOS) *.o
