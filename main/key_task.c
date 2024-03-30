#include "key_task.h"


static const char *TAG = "key";
static QueueHandle_t xQueueKeyHandle = NULL;
static SemaphoreHandle_t xKeySemaphore; //信号量
static touch_event button;              //按键任务队列



uint8_t allow = 1;

static void IRAM_ATTR keyHandler(void *arg)
{
    if(allow == 1)
    {
        uint32_t gpio_num = (uint32_t)arg;
        xQueueSendFromISR(xQueueKeyHandle,&gpio_num,0);
        //xSemaphoreGiveFromISR(xKeySemaphore,pdTRUE);
        allow = 0;
    }
}

void key_init(void)
{
    gpio_set_pull_mode(25,GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(26,GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(27,GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(14,GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(12,GPIO_PULLUP_ONLY);
	gpio_config_t ioConfig = {
		.pin_bit_mask = (1ull << 25)|(1ull << 26)|(1ull << 27)|(1ull << 14)|(1ull << 12),//设置引脚
		.mode = GPIO_MODE_INPUT,
        .pull_down_en = 0 ,     
        .pull_up_en  = 1,
        .intr_type  =  GPIO_INTR_ANYEDGE,  //中断模式下降/上升沿触发
	};
    ESP_LOGI(TAG, "1");
	//传入gpio_config_t指针
    gpio_config(&ioConfig);
    ESP_LOGI(TAG, "1.5");

	gpio_install_isr_service(0);
    ESP_LOGI(TAG, "2");

    gpio_isr_handler_add(25,&keyHandler,(void*)0b11001011);  //3));组合前五位+后三位
    gpio_isr_handler_add(26,&keyHandler,(void*)0b11010000); //0);
    gpio_isr_handler_add(27,&keyHandler,(void*)0b11011001); //1);
    gpio_isr_handler_add(14,&keyHandler,(void*)0b01110100); //4);
    gpio_isr_handler_add(12,&keyHandler,(void*)0b01100010); //2);

    xKeySemaphore =  xSemaphoreCreateBinary();      //创建二进制信号量
    xQueueKeyHandle = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(&key_task, "key_task", 2048, NULL, 5, NULL);
    ESP_LOGI(TAG, "key_init_ok");
}



void key_task(void)
{
    ESP_LOGI(TAG, "key_ruin");
    volatile int num_tack;
    volatile TickType_t tim_a = 0;
    while(1)
    {
        xQueueReceive(xQueueKeyHandle,&num_tack,portMAX_DELAY);
        vTaskDelay(35/ portTICK_PERIOD_MS);
        
        if( key_on == gpio_get_level(num_tack>>3) )
        {
            button.key = (num_tack&0b00000111);
           
            //按键信息发送队列 
            xQueueSendToFront(xQueueWheelHandle,&button,0);
            tim_a = xTaskGetTickCount();
            allow = 1;
        }
        else
        {
            if((xTaskGetTickCount() - tim_a)>200)//1.5s
            {
                button.key = (num_tack&0b00000111) + 5;
                xQueueSendToFront(xQueueWheelHandle,&button,0);
                //长按
            }
            allow = 1;
        } 
        
    }


}