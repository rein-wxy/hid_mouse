#ifndef __BATTERY_TASK_H_
#define __BATTERY_TASK_H_


#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "driver/adc.h"



void battery_init(void);
uint8_t battery_get(void);
#endif