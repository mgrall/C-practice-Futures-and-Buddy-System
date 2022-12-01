#ifndef LIB_H
#define LIB_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Status des Futures
 */
typedef enum {
    PENDING,
    SUCCESS,
    FAILURE
} FutureState;

/**
 * FutureState zu String
 */
extern const char* FUTURE_STATE_STRING[];

/**
 * Das Future
 */
typedef struct {
    FutureState state; /** Status */
    pthread_t thread; /** Thread, in dem die zugehörige Funktion läuft */
    char result[16]; /** Rückgabe der Funktion, sofern fertig und erfolgreich */
} BsFuture;

typedef char* (*BsTask) (void*); // Should return malloc'ed pointer to char array, first 16 bytes are used

/**
 * Funktion asynchron ausführen
 * @param task auszuführende Funktion
 * @param task_arg Parameter für die Funktion
 * @return Future zur asynchronen Ausführung
 */
BsFuture runAsyncTask(BsTask task, void* task_arg);

/**
 * Auf asynchrone Funktion warten
 * @param future Das Future, auf das gewartet werden soll
 * @return Rückgabe der Funktion, soll ein Pointer auf das `result`-Feld des von `future` sein, wenn es nach Abschluss des Threads befüllt wurde
 */
char* awaitTask(BsFuture* future);
#endif
