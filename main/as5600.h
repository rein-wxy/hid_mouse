#ifndef __AS5600_H_
#define __AS5600_H_


#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"

void as5600_task(void);
void as5600_init(void);

extern int total_last ,fuct_b ,fuct_b;
extern QueueHandle_t xQueueWheelHandle;
#endif