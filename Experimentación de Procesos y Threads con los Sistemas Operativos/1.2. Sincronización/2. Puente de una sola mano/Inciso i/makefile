# makefile para compilar el puente de una sola mano con procesos y colas de mensajes.

# Si se ejecuta solo 'make' 'se ejecuta el objetivo all, que incluye todo.
all: PuenteHS

# Compilacion del archivo principal.
PuenteHS:
	gcc PuenteHS.c -o PuenteHS -pthread
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f PuenteHS *.o
