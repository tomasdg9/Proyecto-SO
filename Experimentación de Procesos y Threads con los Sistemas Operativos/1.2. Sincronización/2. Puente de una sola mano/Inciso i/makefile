# makefile para compilar el puente de una sola mano con Hilos y Semáforos.

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: PuenteHS

# Compilacion del puente de una sola mano completo.
PuenteHS:
	gcc PuenteHS.c -o PuenteHS -pthread
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f PuenteHS *.o
