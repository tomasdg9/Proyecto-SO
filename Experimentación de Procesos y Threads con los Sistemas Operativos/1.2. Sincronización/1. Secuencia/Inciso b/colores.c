#include <stdlib.h>
#include <stdio.h>

#define BLACK "\033[1;30m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define WHITE "\033[1;37m"
#define RESET "\033[0m"

/*
 * colores.c: Funciones para impresion de colores por pantalla.
 * -> Cada una de las funciones establece un color especifico para imprimir por pantalla.
 * -> La función reset() permite restablecer el color original de impresión de la consola. 
 */

char * black() {
	return BLACK;
}

char * red() {
	return RED;
}

char * green() {
	return GREEN;
}

char * yellow() {
	return YELLOW;
}

char * blue() {
	return BLUE;
}

char * purple() {
	return PURPLE;
}

char * cyan() {
	return CYAN;
}

char * white() {
	return WHITE;
}

char * reset() {
	return RESET;
}
