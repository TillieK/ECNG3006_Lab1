//ID 816039035 Submitted by Varuna Kistow


#include <stdio.h>
#include "freertos/FreeRTOS.h"

#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/hw_timer.h" // Include hw_timer for timer functionalities

#include "esp_log.h"
#include "esp_timer.h"

// Constants
#define SLOTX   4
#define CYCLEX  5
#define SLOT_T  5000 // 5 sec slot time in ms

#define CONFIG_FREERTOS_HZ 100

int start;

volatile bool timer_done = false;




void timer_callback(void *arg) {
    timer_done = true;
}

void sleep_ms_hw(int ms) {
    timer_done = false;
    hw_timer_alarm_us(ms * 1000, false); // one-shot timer

    while (!timer_done) {
        taskYIELD();  // let other tasks run while waiting
    }

    hw_timer_disarm();
}


// Tasks
void task_one(void) {
    printf("%lu us \n", (unsigned long)esp_timer_get_time());
    printf("Task 1 running\n");
    sleep_ms_hw(1000);
    printf("%lu us \n\n", (unsigned long)esp_timer_get_time());
}

void task_two(void) {
    printf("%lu us \n", (unsigned long)esp_timer_get_time());
    printf("Task 2 running\n");
    sleep_ms_hw(1000);
    printf("%lu us \n\n", (unsigned long)esp_timer_get_time());
}

void task_three(void) {
    printf("%lu us \n", (unsigned long)esp_timer_get_time());
    printf("Task 3 running\n");
    sleep_ms_hw(1000);
    printf("%lu us \n\n", (unsigned long)esp_timer_get_time());
}

void task_four(void) {
    printf("%lu us \n", (unsigned long)esp_timer_get_time());
    printf("Task 4 running\n");
    sleep_ms_hw(1000);
    printf("%lu us \n\n", (unsigned long)esp_timer_get_time());
}

void task_five(void) {
    printf("%lu us \n", (unsigned long)esp_timer_get_time());
    printf("Task 5 running\n");
    sleep_ms_hw(1000);
    printf("%lu us \n\n", (unsigned long)esp_timer_get_time());
}

void burn(void) {
    printf("%lu us \n", (unsigned long)esp_timer_get_time());
    start = esp_timer_get_time();
    sleep_ms_hw(1000);
    printf("Burn time = %ld us\n", (unsigned long)(esp_timer_get_time() - start));
    printf("%lu us \n\n", (unsigned long)esp_timer_get_time());
}


// Task table
void (*ttable[SLOTX][CYCLEX])(void) = {
    {task_one, task_two, burn, burn, burn},
    {task_one, task_three, burn, burn, burn},
    {task_one, task_four, burn, burn, burn},
    {burn, burn, burn, burn, burn}
};


// Main cyclic executive task
void cyclic_executive_task(void *pvParameters) {
    while (1) {
        for (int slot = 0; slot < SLOTX; slot++) {
            for (int cycle = 0; cycle < CYCLEX; cycle++) {
                ttable[slot][cycle]();
            }
        }
    }
}

void app_main(void) {
    printf("Q5 - Cyclic Executive\n");

    hw_timer_init(timer_callback, NULL);

    xTaskCreate(cyclic_executive_task, "cyclic_exec", 4096, NULL, 5, NULL);
}
