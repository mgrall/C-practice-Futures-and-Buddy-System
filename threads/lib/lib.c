#include <lib.h>

const char* FUTURE_STATE_STRING[] = {
    [PENDING] = "PENDING",
    [SUCCESS] = "SUCCESS",
    [FAILURE] = "FAILURE"
};


/**
 * Globale Task Variable. Wird benötigt um die übergebene Funktion zu wrappen,
 * ohne zusätzliche Argumente zu nehmen.
 */
BsTask bs_task;

/**
 * Wrapt die Funktion zu void * _Nullable (void * _Nullable),
 * da pthread_create eine Funktion mit Rückgabewert und Argument vom Typ void* erwartet.
 * @param args Argumente der zu wrappenden Funktion
 * @return void* Return Wert der ursprünglichen Funktion per void pointer.
 */
void *task_wrapper(void *args) {
    void* retval = bs_task(args);
    return retval;
}

BsFuture runAsyncTask(BsTask task, void* task_arg) {
    pthread_t threadId;
    bs_task = task;

    // Benutze wrapper Funktion, um Warnungen zu vermeiden
    int err = pthread_create(&threadId, NULL, task_wrapper, task_arg);
    
    // Checkt ob Thread erfolgreich gestartet wurde
    if (err)
    {
        // Return FAIL Future
        BsFuture fail;
        fail.state = FAILURE;
        fail.thread = threadId;
        return fail;
    }
    // Return PENDING Future
    BsFuture start;
    start.state = PENDING;
    start.thread = threadId;
    return start;    
}

char* awaitTask(BsFuture* future) {
    void *retval;
    // Warte auf Vervollständigung
    int err = pthread_join(future->thread, &retval);
    // Wenn Fehlgeschlagen setze Future Status auf FAILURE
    if (err) future->state = FAILURE;
    else {
        // Lese result
        char* result = retval;    
        char* s;
        int index = 0;
        for (s=&result[0]; *s != '\0'; s++ )
        {
            future->result[index] = *s;
            index = index +1;
        }
        future->state = SUCCESS;
    }
    return future->result;
}
