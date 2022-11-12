#include <stdlib.h>
#include <stdio.h>

/*
 * colores.c: Funciones para impresion de colores por pantalla.
 * -> Cada una de las funciones establece un color especifico para imprimir por pantalla.
 * -> La función reset() permite restablecer el color original de impresión de la consola. 
 */

char * black() {
	return ("\033[1;30m");
}

char * red() {
	return ("\033[1;31m");
}

char * green() {
	return ("\033[1;32m");
}

char * yellow() {
	return ("\033[1;33m");
}

char * blue() {
	return ("\033[1;34m");
}

char * purple() {
	return ("\033[1;35m");
}

char * cyan() {
	return ("\033[1;36m");
}

char * white() {
	return ("\033[1;37m");
}

char * reset() {
	return ("\033[0m");
}
