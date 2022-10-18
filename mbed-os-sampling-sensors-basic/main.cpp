
#include "mbed.h"
#include "stm32l4s5i_iot01_tsensor.h"
#include "stm32l4s5i_iot01_hsensor.h"
#include "stm32l4s5i_iot01_accelero.h"
#include "stm32l4s5i_iot01_gyro.h"
#include "stm32l4s5i_iot01_magneto.h"
#include "stm32l4s5i_iot01_psensor.h"

static BufferedSerial serial_port(USBTX, USBRX, 9600);
static DigitalOut led(LED1);
static DigitalOut led2(LED2);

static LowPowerTicker ledTicker;
static LowPowerTicker readoutTicker;

static bool shouldReadSensors;

FileHandle *mbed::mbed_override_console(int fd)
{
    return &serial_port;
}

static void ledTick()
{
    led = !led;
    led2 = !led;
}

static void readoutTick()
{
    shouldReadSensors = true;
}

static void readSensors()
{
    float temp = BSP_TSENSOR_ReadTemp();
    float humid = BSP_HSENSOR_ReadHumidity();
    float pressure = BSP_PSENSOR_ReadPressure();
    int16_t magnetoXYZ[3] ={0}; 
    BSP_MAGNETO_GetXYZ(magnetoXYZ);
    float gyroXYZ[3] = {0.0}; 
    BSP_GYRO_GetXYZ(gyroXYZ);
    int16_t acceleroXYZ[3] ={0}; 
    BSP_ACCELERO_AccGetXYZ(acceleroXYZ);

    printf("Sensor values:\n");
    printf("* Temperature: %f C\n",temp);
    printf("* Humidity: %f \n", humid );
    printf("* Pressure: %f \n", pressure );
    printf("* Magneto - X: %d Y: %d Z: %d \n", magnetoXYZ[0],magnetoXYZ[1], magnetoXYZ[2]);
    printf("* Gyro - X: %f Y: %f Z: %f \n", gyroXYZ[0], gyroXYZ[1], gyroXYZ[2]);
    printf("* Accelerometer - X: %d Y: %d Z: %d \n", acceleroXYZ[0], acceleroXYZ[1], acceleroXYZ[2]);
}
static void initialiseSensors()
{
    BSP_TSENSOR_Init();
    BSP_HSENSOR_Init();
    BSP_PSENSOR_Init();
    BSP_MAGNETO_Init();
    BSP_ACCELERO_Init();
    BSP_GYRO_Init();
}

int main()
{    
    printf("**** hello - starting sampling ****\n");

    ledTicker.attach(&ledTick, 1s);
    readoutTicker.attach(&readoutTick, 3s);
    initialiseSensors();
    
    /*forever*/
    while (true) {
        sleep();
        if (shouldReadSensors) {
            readSensors();
            shouldReadSensors = false;
        }
    }
}