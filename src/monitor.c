#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>
#include "orchestrator.h"

/**
 * TODO: Implementar la rutina del hilo monitor (Watchdog).
 * * Requisitos:
 * 1. Recuperar la estructura del servicio desde el argumento 'arg'.
 * 2. Utilizar 'waitpid' de forma BLOQUEANTE para esperar al proceso hijo.
 * 3. Analizar el estado de terminación (éxito, error o señal del sistema).
 * 4. Actualizar el Dashboard Global de forma SEGURA (evitar condiciones de carrera).
 */
void* monitor_service(void *arg) {
    // TODO: Castear el argumento al tipo de dato correcto.
    
    service_t *svc = (service_t *)arg;

    int status;

    // TODO: Implementar la espera del proceso específico.
    // Ayuda: Revisar el uso de waitpid(pid, &status, 0).

    //le paso como ultimo argumento 0 (porque no hay opciones)
    //si no hay opciones el proceso padre debe esperar a que el proceso hijo termine para continuar con la ejecución del siguiente código 
    if(waitpid(svc->pid, &status, 0) == -1)
    {
      perror("Error al esperar al proceso hijo");
      return NULL;
    } 

    pthread_mutex_lock(&dashboard_mutex);
    if(WIFEXITED(status))
    {
      if(WEXITSTATUS(status) == 0)
      { 
        //si el proceso terminó normalmente, actualizo el estado a STOPPED y guardo el código de salida
        svc->state = STATE_STOPPED;
      }
      else
      {
        //si el proceso terminó con un error, actualizo el estado a CRASHED y guardo el código de salida
        svc->state = STATE_CRASHED;
      } 
      svc->exit_status = WEXITSTATUS(status);
    }
    else if(WIFSIGNALED(status))
    {
      //si el proceso fue terminado por una señal, actualizo el estado a KILLED y guardo la señal que lo mató
      svc->state = STATE_KILLED;
      svc->exit_status = WTERMSIG(status);
    }
    else
    {
      //en cualquier otro caso, lo marco como CRASHED (puede ser un caso raro pero es para cubrir todas las posibilidades)
      svc->state = STATE_CRASHED;
      svc->exit_status = status; 
    }
    pthread_mutex_unlock(&dashboard_mutex);

    /* * Una vez que waitpid retorna, el proceso hijo ha cambiado de estado.
     * TODO: Analizar el 'status' usando las macros de sys/wait.h:
     * - WIFEXITED: ¿Terminó normalmente?
     * - WEXITSTATUS: ¿Cuál fue su código de retorno?
     * - WIFSIGNALED: ¿Fue terminado por una señal (Segfault, OOM Killer)?
     * - WTERMSIG: ¿Qué señal lo mató?
     */

    /*
     * TODO: Actualizar el dashboard global.
     * ¡CRÍTICO!: El acceso al array 'dashboard' debe estar protegido. 
     * No olvides liberar el mecanismo de sincronización al terminar.
     */

    return NULL;
}
