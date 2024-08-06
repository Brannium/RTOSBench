#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include "porting_layer.h"

#ifdef TRACING
#include "trace.h"
#endif

void no_initialize_test(no_task_entry_t init_function) {
    // Initialize hardware and peripherals, if any (not applicable for spike)
    // Initialize FreeRTOS and start the test task
    init_function(NULL);
    vTaskStartScheduler();
}

no_task_handle_t no_create_task(no_task_entry_t task_entry, char task_name[4], unsigned int prio) {
    no_task_handle_t task_handle;
    xTaskCreate(task_entry, (const char* const)task_name, configMINIMAL_STACK_SIZE, NULL, prio, &task_handle);
    return task_handle;
}

void no_task_yield() {
    taskYIELD();
}

void no_task_suspend(no_task_handle_t task) {
    vTaskSuspend(task);
}

void no_task_suspend_self() {
    vTaskSuspend(NULL);
}

void no_task_resume(no_task_handle_t task) {
    vTaskResume(task);
}

void no_task_delay(unsigned int ticks) {
    vTaskDelay(ticks);
}

void no_init_timer() {
    // Timer initialization not required for FreeRTOS on Spike
}

void no_disable_timer() {
    // Timer disabling not required for FreeRTOS on Spike
}

void no_reset_timer() {
    // Timer reset not required for FreeRTOS on Spike
}

no_time_t no_add_times(const no_time_t* base, unsigned int ticks) {
    no_time_t new_time = *base + ticks;
    return new_time;
}

no_time_t no_time_get() {
    return xTaskGetTickCount();
}

long no_time_diff(const no_time_t* t1, const no_time_t* t2) {
    return (*t2 - *t1);
}

void no_sem_create(no_sem_t* sem, int current_value) {
    *sem = xSemaphoreCreateCounting(50, current_value);
}

void no_sem_wait(no_sem_t* sem) {
    xSemaphoreTake(*sem, portMAX_DELAY);
}

void no_sem_signal(no_sem_t* sem) {
    xSemaphoreGive(*sem);
}

void no_mutex_create(no_mutex_t* mutex) {
    *mutex = xSemaphoreCreateMutex();
}

void no_mutex_acquire(no_mutex_t* mutex) {
    xSemaphoreTake(*mutex, portMAX_DELAY);
}

void no_mutex_release(no_mutex_t* mutex) {
    xSemaphoreGive(*mutex);
}

void no_event_create(no_event_t* event) {
    *event = xEventGroupCreate();
}

void no_event_set(no_event_t* event) {
    xEventGroupSetBits(*event, 0x01);
}

void no_event_reset(no_event_t* event) {
    xEventGroupClearBits(*event, 0x01);
}

void no_event_wait(no_event_t* event) {
    xEventGroupWaitBits(*event, 0x01, pdTRUE, pdFALSE, portMAX_DELAY);
}

void no_mq_create(no_mq_t* mq, unsigned int length, unsigned int msgsize) {
    *mq = xQueueCreate(length, msgsize);
}

void no_mq_send(no_mq_t* mq, unsigned int msg) {
    xQueueSend(*mq, &msg, portMAX_DELAY);
}

void no_mq_receive(no_mq_t* mq) {
    unsigned int msg;
    xQueueReceive(*mq, &msg, portMAX_DELAY);
}

void no_serial_write(const char* string) {
    printf("%s\n", string);
}

void no_cycle_reset_counter() {
    // Counter reset not required for FreeRTOS on Spike
}

unsigned int no_cycle_get_count() {
    return xTaskGetTickCount();
}

void no_single_result_report(char* prefix, int64_t value) {
    printf("%s%lld\n", prefix, value);
}

void no_result_report(int64_t max, int64_t min, int64_t average) {
    printf("max=%lld\nmin=%lld\naverage=%lld\n", max, min, average);
}

#ifdef TRACING
void no_tracing_write_event(int ev_id) {
    // Implement tracing logic here if necessary
}

void no_tracing_report() {
    // Implement tracing report logic here if necessary
}
#endif

