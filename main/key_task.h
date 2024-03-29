#ifndef _KEY_TASK_H_
#define _KEY_TASK_H_

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "as5600.h"
#include "battery_task.h"

#define middle_key 0
#define up_key 1
#define down_key 2
#define left_key 3
#define right_key 4

#define L_middle_key 5
#define L_up_key 6
#define L_down_key 7
#define L_left_key 8
#define right_key 9



#define key_on 0
#define key_off 1

void key_init(void);
void key_task(void);
#endif