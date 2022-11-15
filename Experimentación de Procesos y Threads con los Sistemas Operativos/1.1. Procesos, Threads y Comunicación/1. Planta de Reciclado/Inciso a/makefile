# makefile para compilar la planta de reciclado con Pipes.

# Al ejecutar 'make' se ejecuta el primer objetivo 'all', que incluye la compilación global del ejercicio.
all: PlantaRecicladoPIPES

# Compilacion de la planta de reciclado completa.
PlantaRecicladoPIPES:
	gcc PlantaRecicladoPIPES.c -o PlantaRecicladoPIPES
	
# Regla phony para la eliminacion de archivos .o. (Limpieza post-ejecucion)
clean:
	rm -f PlantaRecicladoPIPES *.o
