#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

/** @brief Dynamically allocates and initializes a new queue with a
 *         maximum size, size
 *
 *  @param size the maximum size of the queue
 *
 *  @return a pointer to a new queue_t
 */

typedef struct queue {
    int in;
    int out;
    int count;
    int size;
    void **buffer;
    pthread_cond_t full;
    pthread_cond_t empty;
    pthread_mutex_t mutex;
} queue;

/*Item buffer[N]
 int in = 0;
 int out = 0;
 int count = 0;
 CV full(L);
 CV empty(L);*/

queue_t *queue_new(int size) {
    queue_t *q;
    q = malloc(sizeof(queue_t));
    q->size = size;
    q->in = 0;
    q->out = 0;
    q->count = 0;
    q->buffer = (void **) malloc(size * sizeof(void *));
    pthread_mutex_init(&(q->mutex), NULL);
    pthread_cond_init(&(q->full), NULL);
    pthread_cond_init(&(q->empty), NULL);
    return q;
}

/** @brief Delete your queue and free all of its memory.
 *
 *  @param q the queue to be deleted.  Note, you should assign the
 *  passed in pointer to NULL when returning (i.e., you should set
 *  *q = NULL after deallocation).
 *
 */
void queue_delete(queue_t **q) {
    free((*q)->buffer);
    pthread_mutex_destroy(&((*q)->mutex));
    pthread_cond_destroy(&((*q)->full));
    pthread_cond_destroy(&((*q)->empty));
    free(*q);
}

/** @brief push an element onto a queue
 *
 *  @param q the queue to push an element into.
 *
 *  @param elem th element to add to the queue
 *
 *  @return A bool indicating success or failure.  Note, the function
 *          should succeed unless the q parameter is NULL.
 */
bool queue_push(queue_t *q, void *elem) {
    if (q == NULL) {
        return false;
    }
    pthread_mutex_lock(&(q->mutex));
    while (q->count == q->size) {
        pthread_cond_wait(&(q->full), &(q->mutex));
    }
    q->buffer[q->in] = elem;
    q->in = (q->in + 1) % q->size;
    q->count += 1;
    pthread_mutex_unlock(&(q->mutex));
    pthread_cond_signal(&(q->empty));
    return true;
}

/** @brief pop an element from a queue.
 *
 *  @param q the queue to pop an element from.
 *
 *  @param elem a place to assign the poped element.
 *
 *  @return A bool indicating success or failure.  Note, the function
 *          should succeed unless the q parameter is NULL.
 */
bool queue_pop(queue_t *q, void **elem) {
    if (q == NULL) {
        return false;
    }
    pthread_mutex_lock(&(q->mutex));
    while (q->count == 0) {
        pthread_cond_wait(&(q->empty), &(q->mutex));
    }
    *(elem) = q->buffer[q->out];

    q->out = (q->out + 1) % q->size;
    q->count -= 1;
    pthread_mutex_unlock(&(q->mutex));
    pthread_cond_signal(&(q->full));
    return true;
}

