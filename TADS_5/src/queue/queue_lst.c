#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "queue/queue_lst.h"

#define QUEUE_CHECK assert(queue != NULL && "bad queue")

// Структура элемента очереди-списка
typedef struct __node
{
    // Указатель на следующий элемент 
    struct __node *next;
    // Указатель на данные
    qdata_t data;
} __node_t;

// Структура очереди-списка
struct __queue_lst
{
    // Размер очереди списка
    unsigned int size;
    // "голова" очереди списка
    __node_t *front;
};

// Создание очереди списка
qu_lst_t qul_create(void)
{
    qu_lst_t queue = calloc(1, sizeof(struct __queue_lst));
    assert(queue && "bad calloc");

    return queue;
}

// Удаление очереди-списка
void qul_destroy(qu_lst_t queue)
{
    QUEUE_CHECK;

    // Освобождаем элементы 
    while (queue->front)
    {
        __node_t *next = queue->front->next;
        free(queue->front);
        queue->front = next;
    }

    // Освобождаем память из-под очереди
    queue->size = 0;
    free(queue);
}

// Добавление в начала очереди-списка
int qul_push_front(qu_lst_t queue, qdata_t data)
{
    QUEUE_CHECK;
    // Выделяем память и добавляем данные
    __node_t *next = queue->front;
    queue->front = malloc(sizeof(__node_t));
    assert(queue->front && "bad malloc");
    queue->front->data = data;
    queue->front->next = next;
    queue->size++;
    return SUCCESS;
}

// Вывод из конца очереди
int qul_pop_back(qu_lst_t queue, qdata_t *data)
{
    QUEUE_CHECK;

    int status = QUEUE_EMPTY;
    // Если размер очереди не равен 0 то
    if (queue->size > 0)
    {
        // Доходим до последнего элемента
        __node_t **node = &queue->front;
        while ((*node)->next != NULL)
            node = &(*node)->next;
        // Извлекаем данные
        *data = (*node)->data;
        // Удаляем последний элемент
        free(*node);
        (*node) = NULL;
        queue->size--;
    }

    return status;
}

// Вычисляем размер очереди с проверкой
unsigned int qul_get_size(qu_lst_t queue)
{
    QUEUE_CHECK;

    return queue->size;
}

// Возвращаем значение элемента по индексу
qdata_t *qul_at(qu_lst_t queue, unsigned int index)
{
    QUEUE_CHECK;

    assert(index < queue->size && "bad index");
    __node_t *node = queue->front;

    index = queue->size - index;
    while (--index > 0)
        node = node->next;

    return &node->data;
}
