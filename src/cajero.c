#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include "../include/logger.h"
#include "../include/mensaje_gustos.h"
#include "../include/mensaje_ticket.h"
#include "../include/ICajeroMOM.h"

#define CAJERO 		"CAJERO\t\t" 
#define MIN_TIME	3

int main(int argc, char** argv) {
	Logger log = crearLogger();
	pid_t pid = getpid();
	char buffer[BUFFER_SIZE];
		
	escribirLog(&log,DEBUG,pid,CAJERO,"Hola soy el cajero, voy a empezar a escuchar clientes");

	int ticket = 1;

	// Para saber si tengo que eliminar IPC
	bool meVoy = false;

    //Cajero Handler
    Cajero_handler handler = registrarCajero();
	Mensaje_gustos msg_gustos;
	Mensaje_ticket msg_ticket;

	//init de random
	generarNumeroRandomConSeed(pid,pid);

	while (!meVoy) {

        recibirPedido(&handler,&msg_gustos);
		sprintf(buffer,"Recibi un mensaje de %d :O",msg_gustos.id);
		escribirLog(&log,DEBUG,pid,CAJERO,buffer);

		if (esMsgDeIrse(&msg_gustos)) {
            enviarMsgQueMeVoy(&handler);
			escribirLog(&log,DEBUG,pid,CAJERO,"Voy a irme, nos vemos");
			meVoy = true;
		} else {
			crearMsgTicket(&msg_ticket,msg_gustos.id,ticket);

			escribirLog(&log,DEBUG,pid,CAJERO,"Le voy a pasar al cliente su ticket pero primero voy a tardar");
            sleep(generarNumeroRandomConMin(MIN_TIME,MIN_TIME));
            enviarTicketACliente(&handler,&msg_ticket);

			escribirLog(&log,DEBUG,pid,CAJERO,"Le paso a los heladeros el pedido");
            enviarPedidoAHeladero(&handler,&msg_gustos,ticket);

			ticket = (ticket + 1) % INT_MAX;
		}
	}

	cerrarCajero(&handler);
	cerrarLogger(&log);

	return 0;
}