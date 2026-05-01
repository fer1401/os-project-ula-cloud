#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "orchestrator.h"

/**
 * TODO: Implementar el despliegue del microservicio.
 * * Requisitos:
 * 1. Crear un nuevo proceso.
 * 2. En el proceso HIJO:
 * - Configurar las restricciones de recursos (memoria).
 * - Reemplazar la imagen del proceso por el binario del servicio.
 * 3. En el proceso PADRE:
 * - Registrar el PID y el estado inicial en el dashboard.
 * - Retornar el PID asignado.
 */
int spawn_service(int index) {
    pid_t pid = fork();

    if(pid < 0)
    {
        perror("Error al crear el proceso hijo");
        return -1; 
    }
    else if(pid == 0) // Proceso HIJO
    {
        apply_resource_limits(dashboard[index].mem_limit); 

        char *service_path = dashboard[index].path;//rutas de los servicios (0 = logger, 1 = chaos, 2 = leak)
        char *service_name = dashboard[index].name;
        //hago el casteo para que el segundo argumento de execvp sea un array de char* con el nombre del servicio y un NULL al final(fin de argumentos)
        execvp(service_path, (char *[]){service_name, NULL});  

        //si execvp falla, continua la ejecución de la imagen en la que estaba
        perror("Error al ejecutar el servicio");
        exit(EXIT_FAILURE);
    }
    else // Proceso PADRE
    {
        //mutex lock para asegurarme de que exista solo un proceso escribiendo en el dashboard
        pthread_mutex_lock(&dashboard_mutex); 
        dashboard[index].pid = pid;
        dashboard[index].state = STATE_RUNNING; 
        //unlock para que otros procesos puedan escribir en el dashboard en la siguiente iteración
        pthread_mutex_unlock(&dashboard_mutex);
    }

    // TODO: Invocar la creación del proceso hijo.

    // Casos a manejar:
    // - Error en la creación del proceso.
    // - Lógica del proceso HIJO (Setup de límites y Ejecución).
    // - Lógica del proceso PADRE (Gestión del dashboard).

    return pid; // Cambiar por el PID real
}
