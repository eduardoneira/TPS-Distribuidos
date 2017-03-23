#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#include "../include/msg_queue.h"
#include "../include/lockfile.h"
#include "../include/fifo_lectura.h"
#include "../include/fifo_escritura.h"
#include "../include/constructor.h"

int main (int argc, char ** argv){
	
	crearIPC();

	for (int i = 0; i < 10; i++){
		if (fork() == 0){
			execl("./cliente","./cliente",(char*) NULL);
			perror("Exec fallo");
			return -1;
		}		
	}

	sleep(generarNumeroRandom());

	return 0;
} 	