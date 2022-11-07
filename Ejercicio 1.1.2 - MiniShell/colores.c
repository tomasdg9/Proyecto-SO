#include <stdlib.h>
#include <stdio.h>

/*
 * colores.c: Funciones para impresion de colores por pantalla.
 * -> Cada una de las funciones establece un color especifico para imprimir por pantalla.
 * -> La función reset() permite restablecer el color original de impresión de la consola. 
 */

void black() {
	printf("\033[1;30m");
}

void red() {
	printf("\033[1;31m");
}

void green() {
	printf("\033[1;32m");
}

void yellow() {
	printf("\033[1;33m");
}

void blue() {
	printf("\033[1;34m");
}

void purple() {
	printf("\033[1;35m");
}

void cyan() {
	printf("\033[1;36m");
}

void white() {
	printf("\033[1;37m");
}

void reset() {
	printf("\033[0m");
}
