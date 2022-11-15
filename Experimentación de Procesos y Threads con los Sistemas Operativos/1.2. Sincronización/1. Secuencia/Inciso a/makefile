# makefile para compilar la secuencia con Hilos y Semaforos.

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: SecuenciaHS

# Compilacion del archivo principal.
SecuenciaHS:
	gcc SecuenciaHS.c -o SecuenciaHS -pthread
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f SecuenciaHS *.o
