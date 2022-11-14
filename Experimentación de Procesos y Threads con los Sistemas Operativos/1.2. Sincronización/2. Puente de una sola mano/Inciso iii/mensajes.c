#include <string.h>

#define SIZE_MENSAJE sizeof(MENSAJE) - sizeof(long)
#define CONTADOR_PUENTE 5
#define Puente 1
#define PasandoN 2
#define PasandoS 3
#define MutexN 4
#define MutexS 5
#define CoordinadorN 6
#define CoordinadorS 7

/*
 * struct utilizado para la representacion de las señales del puente.
 * Son utilizados como Mensajes en las operaciones de las MQs.
 **/
typedef struct mensaje {
	long tipo;
    char msg[20];
} MENSAJE;

MENSAJE crearMensaje(long tipo) {
	MENSAJE m;
	m.tipo = tipo;
    
	switch(tipo) {
		case Puente: strcpy(m.msg, "Puente"); break;
		case PasandoN: strcpy(m.msg, "NortePuente"); break;
		case PasandoS: strcpy(m.msg, "SurPuente"); break;
		case MutexN: strcpy(m.msg, "MutexN"); break;
		case MutexS: strcpy(m.msg, "MutexS"); break;
		case CoordinadorN: strcpy(m.msg, "CoordinadorN"); break; 
		case CoordinadorS: strcpy(m.msg, "CoordinadorS"); break;
	}
	
	return m;
}
