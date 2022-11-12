# makefile para compilar el puente de una sola mano con procesos y colas de mensajes.

# Si se ejecuta solo 'make' 'se ejecuta el objetivo all, que incluye todo.
all: PuentePMQ VehiculoNORTE VehiculoSUR

# Compilacion del archivo principal.
PuentePMQ:
	gcc PuentePMQ.c -o PuentePMQ

VehiculoNORTE:
	gcc VehiculoNORTE.c -o VehiculoNORTE
	
VehiculoSUR:
	gcc VehiculoSUR.c -o VehiculoSUR
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f PuentePMQ VehiculoNORTE VehiculoSUR *.o
