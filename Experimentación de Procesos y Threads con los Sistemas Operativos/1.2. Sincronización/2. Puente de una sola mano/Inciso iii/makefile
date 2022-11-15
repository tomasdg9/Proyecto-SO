# makefile para compilar el puente de una sola mano con Procesos y Colas de Mensajes.

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: PuentePMQ VehiculoNORTE VehiculoSUR

# Compilacion del archivo principa del puente de una sola mano.
PuentePMQ:
	gcc PuentePMQ.c -o PuentePMQ

# Compilacion del archivo para los vehículos del NORTE.
VehiculoNORTE:
	gcc VehiculoNORTE.c -o VehiculoNORTE
	
# Compilacion del archivo para los vehículos del NORTE.
VehiculoSUR:
	gcc VehiculoSUR.c -o VehiculoSUR
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f PuentePMQ VehiculoNORTE VehiculoSUR *.o
