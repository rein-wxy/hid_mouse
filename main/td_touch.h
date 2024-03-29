#ifndef _TD_TOUCH_H_
#define _TD_TOUCH_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
//#include "driver/mcpwm.h"//电机LEDPWM
//#include "driver/mcpwm_prelude.h"


extern uint8_t  device_mode ;
void touch_uer(void);
void touch_uer_task(void);
#endif