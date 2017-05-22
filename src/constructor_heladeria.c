#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "../include/constructor.h"
#include "../include/mensaje_ticket.h"
#include "../include/mensaje_helado.h"
#include "../include/mensaje_gustos.h"

void execMOM(char* modo) {
	char size[20];

	//Lanzo al MOM REGISTER
	if (fork() == 0) {
		execl("./MOM_register","./MOM_register",modo,(char*) NULL);
		perror("Exec fallo");
		exit(-1);
	}

	if (strcmp(modo,ALL) || strcmp(modo,CAJERO) == 0) {
		//Lanzo al MOM CAJERO-HELADERO
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_gustos));
			execl("./MOM_server","./MOM_server",CAJERO,HELADERO,size,CAJERO,"16000",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

		//Lanzo al MOM CLIENTE-CAJERO
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_gustos));
			execl("./MOM_server","./MOM_server",CLIENTE,CAJERO,size,CAJERO,"16001",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

		//Lanzo al MOM CAJERO-CLIENTE
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_ticket));
			execl("./MOM_server","./MOM_server",CAJERO,CLIENTE,size,CAJERO,"16002",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}
	} else if (strcmp(modo,ALL) || strcmp(modo,HELADERO) == 0){
		//Lanzo al MOM CAJERO-HELADERO
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_gustos));
			execl("./MOM_server","./MOM_server",CAJERO,HELADERO,size,HELADERO,"18000",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

		//Lanzo al MOM HELADERO-CLIENTE
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_helado));
			execl("./MOM_server","./MOM_server",HELADERO,CLIENTE,size,HELADERO,"18001",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}
	} else if (strcmp(modo,ALL) || strcmp(modo,CLIENTE) == 0) {
		//Lanzo al MOM CLIENTE-CAJERO
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_gustos));
			execl("./MOM_server","./MOM_server",CLIENTE,CAJERO,size,CLIENTE,"20001",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

		//Lanzo al MOM CAJERO-CLIENTE
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_ticket));
			execl("./MOM_server","./MOM_server",CAJERO,CLIENTE,size,CLIENTE,"20002",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}

		//Lanzo al MOM HELADERO-CLIENTE
		if (fork() == 0) {
			sprintf(size,"%zu",sizeof(Mensaje_helado));
			execl("./MOM_server","./MOM_server",HELADERO,CLIENTE,size,CLIENTE,"20003",(char*) NULL);
			perror("Exec fallo");
			exit(-1);
		}
	}

}

int main (int argc, char ** argv){

	if (argc == 1) {
		printf("Se debe especificar que crear, se pueden usar:\n heladero\n cajero\n cliente\n all\n");
		return 1;
	}

	crearIPC(argv[1]);

	execMOM(argv[1]);

	if (strcmp(argv[1],ALL) == 0 || strcmp(argv[1],CAJERO) == 0) {
		//Lanzo al cajero
		for (int i = 0; i < CANT_CAJEROS; i++) {
			if (fork() == 0) {
				execl("./cajero", "./cajero", (char *) NULL);
				perror("Exec fallo");
				return -1;
			}
		}
	}

	if (strcmp(argv[1],ALL) == 0 || strcmp(argv[1],HELADERO) == 0) {
		// Lanzo a los heladeros
		for (int i = 0; i < CANT_HELADEROS; i++){
			if (fork() == 0) {
				execl("./heladero", "./heladero", (char *) NULL);
				perror("Exec fallo");
				return -1;
			}
		}

	}


	return 0;
} 	