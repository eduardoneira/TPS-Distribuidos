# TP3-Distribuidos
TP 3 de Distribuidos : Middleware Distribuido

# Consigna

Hacer una simulación de una heladería utilizando IPC. Crear dos procesos 'heladeros' y un proceso 'cajero'. El proceso cajero toma pedidos de procesos 'cldiente' que van llegando a la heladeria. Luego atender al cliente y cobrarle el pedido, el cajero pasa los datos del pedido a uno de los heladeros y entrega un vale al cliente. El heladero sirve helado de los gustos elegidos por el cliente y al finalizar se lo entrega al cliente que tenga en posicion el vale correspondiente. Al obtener el helado, el cliente ocupa un lugar en la heladeria hasta que termina de consumir el helado. Al finalizar se retira del lugar.

Suposiciones:
- Los clientes siempre eligen 3 gustos de helado.
- El cajero tarda un tiempo aleatorio en atender a cada cliente.
- El heladero tarda un tiempo aleatorio en servir cada gusto de helado.
- El cliente tarda un tiempo aleatorio en consumir el helado.
- Dos heladeros no pueden acceder al mismo tiempo al mismo gusto de helado.
- La heladeria tiene N lugares disponibles para sentarse. Si se encuentra llena, un cliente puede elegir irse o pedir un helado para llevar. Los clientes no esperan a que haya lugres libres.
- La heladeria tiene M lugares disponibles para que los clientes esperen a ser atendidos. Si no hay mas lugar, los clientes que llegan no se ponen en la cola. Se van.
- Al cerrar el negocio, se le permite a los clientes quedarse dentro del establecimiento hasta finalizar su helado. Luego deben retirarse. No pueden ingresar nuevos clientes.

# Comandos

IMPORTANTE

Primero configurar archivo ips.txt con las ips de donde se van a encontrar cada rol. Acordarse de tener el archivo en el mismo lugar donde se encuentran las ips.

Antes de correr hay que levantar el server RPC. Para hacer esto:

```
cd rpc
make                    // compila el server rpc
rpcbind					// En caso de no tener levanto rpc
sudo ./server_rpc		// Importante que sea con permisos de root
```

El orden para levantar los MOM debe ser el siguiente. Primero cajero, después heladero y finalmente cliente. Estos se levantan lanzando el constructor por lo que se debe correr primero el constructor cajero, luego el constructor heladero y por último el constructor cliente. Si se lanza con el all esto ya se tiene en cuenta pero acordarse las ips deben estar en localhost ya que se correrá todo en la misma computadora. 

```
cmake .			  				// Crea el make para
make                      		// compila el codigo y genera los ejecutables
./constructor_heladeria [modo]	// lanza un proceso cajero y dos procesos heladeros con forks, tambien lanza los MOMS. El modo puede ser "cliente", "cajero", "heladero" o "all".
./cliente 						// lanza un cliente. Debe haberse lanzado el constructor antes
./heladero  					// lanza un heladero. Debe haberse lanzado el constructor antes
./cajero 						// lanza un cajero. Debe haberse lanzado el constructor antes
./control 5               		// lanza 5 (o el numero ingresado) procesos cliente que actuan segun la consigna
./control                 		// si no recibe un numero de clientes como argumento, control cambia el estado de la heladeria a cerrada (pero no destruye ningun mecanismo de IPC)
./destructor_heladeria [modo]  	// envia mensajes a los procesos que se bloquean escuchando mensajes para que se cierren, y tambien destruye las estructuras de IPC creadas. No hace nada si la heladeria no esta cerrada o todavia hay clientes comiendo. El modo puede ser "cliente", "cajero", "heladero" o "all"
```
