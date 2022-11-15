# makefile para compilar la planta de reciclado con Cola de Mensajes.

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: PlantaRecicladoMQ Recolector Clasificador Reciclador

# Compilacion del archivo principal de la planta de reciclado.
PlantaRecicladoMQ:
	gcc PlantaRecicladoMQ.c -o PlantaRecicladoMQ
	
# Compilacion del archivo recolector.
Recolector:
	gcc Recolector.c -o Recolector
	
# Compilacion del archivo clasificador.
Clasificador:
	gcc Clasificador.c -o Clasificador

# Compilacion del archivo recolector.
Reciclador:
	gcc Reciclador.c -o Reciclador
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f PlantaRecicladoMQ Recolector Clasificador Reciclador *.o
