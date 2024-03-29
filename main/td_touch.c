#include "td_touch.h"

#define LEDC_HS_MODE  LEDC_LOW_SPEED_MODE
#define LEDC_HS_TIMER LEDC_TIMER_0	
#define LEDC_TOTAL_NUM         1				// pwm个数

#define LEDC_HS_CH0_GPIO       22				// pwm控制器通道对应GPIO
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0	// LED控制器通道号

void pwm_uer_init(void)
{
    gpio_config_t ioConfig = {
		.pin_bit_mask = (1ull << 18)|(1ull << 22),//设置引脚
		.mode = GPIO_MODE_OUTPUT,
        .pull_down_en = 1,     
        .pull_up_en  = 0,
        .intr_type  =  GPIO_INTR_DISABLE,  //中断模式下降/上升沿触发
	};
    gpio_config(&ioConfig);

    ledc_timer_config_t ledc_timer_Structure;
    ledc_timer_Structure.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_timer_Structure.timer_num = LEDC_TIMER_0;
    ledc_timer_Structure.duty_resolution = 10; // 分辨率0-1024  
    ledc_timer_Structure.freq_hz = 5000;
    ledc_timer_Structure.clk_cfg = LEDC_APB_CLK;

    ledc_timer_config(&ledc_timer_Structure);

    ledc_channel_config_t ledc_channel_Structure;
    ledc_channel_Structure.gpio_num = 19 ; // gpio 12
    ledc_channel_Structure.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel_Structure.channel = LEDC_CHANNEL_0;
    ledc_channel_Structure.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_Structure.timer_sel = 0;
    ledc_channel_Structure.duty = 0;
    ledc_channel_Structure.hpoint = 0;
    ledc_channel_config(&ledc_channel_Structure);

    ledc_channel_config_t ledc_channel_Structure_2;
    ledc_channel_Structure_2.gpio_num = 23 ; // gpio 12
    ledc_channel_Structure_2.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel_Structure_2.channel = LEDC_CHANNEL_1;
    ledc_channel_Structure_2.intr_type = LEDC_INTR_DISABLE;
    ledc_channel_Structure_2.timer_sel = 0;
    ledc_channel_Structure_2.duty = 0;
    ledc_channel_Structure_2.hpoint = 0;
    ledc_channel_config(&ledc_channel_Structure_2);

    ledc_fade_func_install(ESP_INTR_FLAG_LEVEL1);

   
    // 若想改变占空比，用这个。
    // ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, 8190 * 0.4); // 负占空比，负值为50/256=19.5%; 0-8190
    // ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}
/*发送LEDPWM方波*/
void ledPwm_update(float a,float b)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, a*1023); //0-1024 //负占空比，负值为50/256=19.5%; 0-8190
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1, b*1023); 
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
}

/*
void mpwm_uer_init(void)
{
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 22); // 一台 MCPWM 发生器就足够了
    mcpwm_config_t pwm_config = {
        .frequency = 2000, // 频率=2kHz
        .cmpr_a = 0,       // PWMxA 的占空比 = 0
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

}
*/

void touch_uer_task(void)
{
    gpio_set_level(18,1);
    while(1)
    {
        //ledPwm_update(0,1);
        //vTaskDelay(2000/ portTICK_PERIOD_MS);
        //ledPwm_update(1,0);
        //vTaskDelay(2000/ portTICK_PERIOD_MS);
        gpio_set_level(22,0);
        vTaskDelay(500/ portTICK_PERIOD_MS);
        if(device_mode == 0)
            gpio_set_level(22,0);
        else
            gpio_set_level(22,1);
        vTaskDelay(500/ portTICK_PERIOD_MS);
    }
}
void touch_uer(void)
{
    pwm_uer_init();
    
    //xTaskCreate(&touch_uer_task, "touch_task", 128, NULL, 5, NULL);

}



