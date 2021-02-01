#include <stdlib.h>
#include <assert.h>
#include "status_codes.h"
#include "queue/queue_arr.h"
// Проверка очереди
#define QUEUE_CHECK assert(queue != NULL && "bad queue")

// Структура для описания очереди-массива
struct __queue_arr
{
    // Размер очереди 
    unsigned int size;
    // Максимальное количество элементов
    unsigned int capacity;

    // Pin и Pout
    unsigned int front_index;
    unsigned int back_index;

    // Указатель на очередь
    qdata_t *data;
};

// Создание очереди-массива
qu_arr_t qua_create(unsigned int size)
{
    // Выделение и инициализация памяти под очередь-массив
    qu_arr_t queue = calloc(1, sizeof(struct __queue_arr));
    assert(queue && "bad array queue allocation");
    // Инициализация очереди-массива
    queue->data = malloc(size * sizeof(qdata_t));
    if (queue->data != NULL)
        queue->capacity = size;

    return queue;
}

// Освобождение очереди
void qua_destroy(qu_arr_t queue)
{
    // Проверка очереди на корректность
    QUEUE_CHECK;

    // Проверка очереди на "освобождённость" и освобождение если она не пустая
    if (queue->data != NULL)
    {
        free(queue->data);
        queue->data = NULL;
        free(queue);
    }
}

// Добавление элемента в начало очереди
int qua_push_front(qu_arr_t queue, qdata_t data)
{
    // Проверка очереди
    QUEUE_CHECK;

    // Статус очреди
    int status = QUEUE_OVERFLOW;
    // Если размер меньше чем максимальное количество элементов в очереди
    if (queue->size < queue->capacity)
    {
        // Добавляем элемент
        queue->data[queue->front_index] = data;
        // Увеличиваем размер очереди
        queue->size++;
        // Вычисляем положение Pin
        queue->front_index = (queue->front_index + 1) % queue->capacity;
        status = SUCCESS;
    }

    return status;
}

// Вывод элемента из конца очереди
int qua_pop_back(qu_arr_t queue, qdata_t *data)
{
    // Проверка очереди
    QUEUE_CHECK;

    int status = QUEUE_EMPTY;
    // Если размер не равен 0
    if (queue->size > 0)
    {
        // Вынимаем элемент по Pout
        *data = queue->data[queue->back_index];
        // Уменьшаем переменную размера
        queue->size--;
        // Вычисляем положение Pout
        queue->back_index = (queue->back_index + 1) % queue->capacity;
        status = SUCCESS;
    }

    return status;
}

// Вычисляем размер очереди с проверкой
unsigned int qua_get_size(qu_arr_t queue)
{
    QUEUE_CHECK;

    return queue->size;
}

// Возвращаем значение элемента по индексу
qdata_t *qua_at(qu_arr_t queue, unsigned int index)
{
    QUEUE_CHECK;

    assert(index < queue->size && "bad index");
    return queue->data + (index + queue->back_index) % queue->capacity;
}
