# makefile para compilar la minishell

# Si se ejecuta solo 'make' 'se ejecuta el objetivo all, que incluye todo.
all: PlantaRecicladoMQ Recolector Clasificador Reciclador

# Compilacion de la planta de reciclado.
PlantaRecicladoMQ:
	gcc PlantaRecicladoMQ.c -o PlantaRecicladoMQ
	
# Compilacion del recolector.
Recolector:
	gcc Recolector.c -o Recolector
	
# Compilacion del clasificador.
Clasificador:
	gcc Clasificador.c -o Clasificador

# Compilacion del recolector.
Reciclador:
	gcc Reciclador.c -o Reciclador
	
clean:
	rm -f PlantaRecicladoMQ Recolector Clasificador Reciclador *.o
