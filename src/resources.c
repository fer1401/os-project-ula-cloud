#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include "orchestrator.h"

/**
 * TODO: Implementar la restricción de recursos del sistema.
 * * Requisitos:
 * 1. Definir los límites (soft y hard) para el espacio de direccionamiento virtual.
 * 2. Aplicar el límite utilizando la syscall 'setrlimit'.
 * 3. Garantizar que si la configuración falla, el proceso no continúe su ejecución.
 * * Ayuda: Revisar la constante RLIMIT_AS en la documentación de Linux.
 */
void apply_resource_limits(size_t mem_limit) {
    struct rlimit rl;

    rl.rlim_cur = mem_limit; // Límite soft
    rl.rlim_max = mem_limit; // Límite hard

    if(setrlimit(RLIMIT_AS, &rl) == -1) 
    {
        perror("Error al establecer límites de recursos");
        exit(EXIT_FAILURE);
    }
    
    // TODO: Configurar la estructura rlimit y ejecutar la syscall.
    
    // Casos a considerar:
    // - ¿Qué diferencia hay entre rlim_cur y rlim_max?
    //rlim_cur (soft), limite actual que el proceso puede usar y sobrepasar (quedando "vivo" pero con una advertencia)
    //rlim_max (hard), limite máximo que el proceso puede usar pero no sobrepasar (no lanza advertencia, "mata" el proceso directamente)
    // - ¿Qué sucede si el límite solicitado es menor al tamaño del propio binario?
    //si el limite es menor al tamaño del binario, el proceso no puede cargar por completo la imagen en memoria(lanza error de memoria insuficiente)
}
