# makefile para compilar la secuencia con Procesos y Pipes.

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: SecuenciaPP

# Compilacion del archivo principal.
SecuenciaPP:
	gcc SecuenciaPP.c -o SecuenciaPP

# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)	
clean:
	rm -f SecuenciaPP *.o
