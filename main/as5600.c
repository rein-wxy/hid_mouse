/* i2c - Simple example

   Simple I2C example that shows how to initialize I2C
   as well as reading and writing from and to registers for a sensor connected over I2C.

   The sensor used in this example is a MPU9250 inertial measurement unit.

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "as5600.h"


static const char *TAG = "i2c-simple-example";

#define I2C_MASTER_SCL_IO           4//CONFIG_I2C_MASTER_SCL      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           0//CONFIG_I2C_MASTER_SDA      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          100000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

#define MPU9250_SENSOR_ADDR                 0x68        /*!< Slave address of the MPU9250 sensor */
#define MPU9250_WHO_AM_I_REG_ADDR           0x75        /*!< Register addresses of the "who am I" register */

#define MPU9250_PWR_MGMT_1_REG_ADDR         0x6B        /*!< Register addresses of the power managment register */
#define MPU9250_RESET_BIT                   7

#define AS5600_SENSOR_ADDR                 0x36
#define RawAngle_Addr                      0x0C 

QueueHandle_t xQueueWheelHandle;//滚轮队列

/**
 * @brief Read a sequence of bytes from a MPU9250 sensor registers
 */
static esp_err_t mpu9250_register_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    return i2c_master_write_read_device(I2C_MASTER_NUM, AS5600_SENSOR_ADDR , &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

/**
 * @brief Write a byte to a MPU9250 sensor register
 */
static esp_err_t mpu9250_register_write_byte(uint8_t reg_addr, uint8_t data)
{
    int ret;
    uint8_t write_buf[2] = {reg_addr, data};

    ret = i2c_master_write_to_device(I2C_MASTER_NUM, AS5600_SENSOR_ADDR , write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    return ret;
}

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}




int total_last = 0,total_err = 0;
void as5600_task(void)
{
    volatile uint8_t data[2];
    volatile static int angle = 0,last_angle = 0,total_angle = 0,angle_err = 0;
    xQueueWheelHandle = xQueueCreate(10, sizeof( int8_t));  //创建滚轮数据队列

    int8_t lValueToSend;

    //ESP_ERROR_CHECK(i2c_master_init());
    //ESP_LOGI(TAG, "I2C initialized successfully");

    /* Read the MPU9250 WHO_AM_I register, on power up the register should have the value 0x71 */
    while (1)
    {
        ESP_ERROR_CHECK(mpu9250_register_read(RawAngle_Addr, data, 2));
        angle = data[0]<<8|data[1];
        //ESP_LOGI(TAG, "WHO_AM_I = %d", data[0]<<8|data[1]);
        angle_err = angle -last_angle;
        if(angle_err>-4096 && angle_err<-2048)//正转
            angle_err = 4096 - last_angle + angle;
        if(angle_err < 4096 && angle_err > 2048)//反转
            angle_err = angle - 4096 - last_angle;
        total_angle += angle_err; 
        last_angle = angle;
        //ESP_LOGI(TAG, "WHO_AM_I = %d ,  %d", total_angle,angle_err);
        total_err = total_angle - total_last;
        if( (total_err >= 256))
        {
            total_last = total_angle;
            lValueToSend = 10; 
            xQueueSendToFront(xQueueWheelHandle,&lValueToSend,0);
        }
        else if((total_err <= -256))
        {
            total_last = total_angle;
            lValueToSend = 11;
            xQueueSendToFront(xQueueWheelHandle,&lValueToSend,0);
        }
        else
        { 
            lValueToSend = 0;
            // xQueueSendToFront(xQueueWheelHandle,&lValueToSend,0);
        }
        //ESP_LOGI(TAG, "WHO_AM_I = %d ,  %d", total_angle,angle_err);
        data[0] = 0;
        data[1] = 0;
        vTaskDelay(20/portTICK_PERIOD_MS);
        /* code */
    }
    

    /* Demonstrate writing by reseting the MPU9250 */
    //ESP_ERROR_CHECK(mpu9250_register_write_byte(RawAngle_Addr, 1 << MPU9250_RESET_BIT));

    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
    ESP_LOGI(TAG, "I2C de-initialized successfully");
}


void as5600_init(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");
    //xTaskCreate(&as5600_task, "as5600_task", 2048, NULL, 5, NULL);
}