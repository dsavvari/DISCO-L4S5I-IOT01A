
#include "mbed.h"
#include "stm32l4s5i_iot01_tsensor.h"
#include "stm32l4s5i_iot01_hsensor.h"
#include "stm32l4s5i_iot01_accelero.h"
#include "stm32l4s5i_iot01_gyro.h"
#include "stm32l4s5i_iot01_magneto.h"
#include "stm32l4s5i_iot01_psensor.h"
#include "ble/BLE.h"
#include "ble_process.h"
#include <initializer_list>

class Lab3ServerProcess : public BLEProcess {
public:
    Lab3ServerProcess(events::EventQueue &event_queue, BLE &ble_interface): BLEProcess(event_queue, ble_interface) {
    }

    const char *get_device_name() override {
        static const char name[] = "Lab 3 Server";
        return name;
    }
};

class SensorReadingCharacteristic : public GattCharacteristic {
public:
    SensorReadingCharacteristic(const UUID &uuid): GattCharacteristic(uuid, (uint8_t *)&stringRepr_, sizeof(stringRepr_),
    sizeof(stringRepr_), GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY), 
    internalValue_(0) {
        updateStringRepr();
    }

    void updateValue(BLE &ble, float newValue) {
        internalValue_ = newValue;
        updateStringRepr();

        ble.gattServer().write(getValueHandle(), (uint8_t *)&stringRepr_, sizeof(stringRepr_));
    }

private:
    float internalValue_;
    uint8_t stringRepr_[16];

    void updateStringRepr() {
        sprintf((char *)stringRepr_, "%f", internalValue_);
    }
};


class Lab3Server : ble::GattServer::EventHandler {
public:
    Lab3Server() :  temperature_(GattCharacteristic::UUID_TEMPERATURE_CHAR),
                    humidity_(GattCharacteristic::UUID_HUMIDITY_CHAR),
                    pressure_(GattCharacteristic::UUID_PRESSURE_CHAR), 
                    led_(LED1), 
                    ledValue_(0), 
                    ledChar_(0xA000, &ledValue_) {

        initialiseSensors();
    }
    virtual ~Lab3Server() {}

    void start(BLE &ble, events::EventQueue &event_queue) {
        const UUID uuid = GattService::UUID_ENVIRONMENTAL_SERVICE;
        GattCharacteristic *charTable[] = {&temperature_, &humidity_, &pressure_, &ledChar_};
        GattService sensorService(uuid, charTable,sizeof(charTable) / sizeof(charTable[0]));

        ble.gattServer().addService(sensorService);
        ble.gattServer().setEventHandler(this);

        event_queue.call_every(5000ms, [this, &ble] { updateSensors(ble); });

        printf("Service started.\n");

    }

    virtual void onDataWritten(const GattWriteCallbackParams &params) override {
        if ((params.handle == ledChar_.getValueHandle()) && (params.len == 1)) {
        printf("New LED value: %x\r\n", *(params.data));
        ledValue_ = *(params.data);
        led_ = ledValue_;
        }
    }

    void initialiseSensors() {
        BSP_TSENSOR_Init();
        BSP_HSENSOR_Init();
        BSP_PSENSOR_Init();
    }


    void updateSensors(BLE &ble) {

        printf("Updating sensors...\n");
        float temp = BSP_TSENSOR_ReadTemp();
        float humid = BSP_HSENSOR_ReadHumidity();
        float pressure = BSP_PSENSOR_ReadPressure();
        printf("Sensor values - * Temperature: %f C * Humidity: %f  * Pressure: %f  \n",temp, humid, pressure );
        temperature_.updateValue(ble, temp);
        humidity_.updateValue(ble, pressure);
        pressure_.updateValue(ble, pressure);
    }

private:
    SensorReadingCharacteristic temperature_;
    SensorReadingCharacteristic humidity_;
    SensorReadingCharacteristic pressure_;
    DigitalOut led_;
    uint8_t ledValue_;
    ReadWriteGattCharacteristic<uint8_t> ledChar_;
};


static EventQueue event_queue(10 * EVENTS_EVENT_SIZE);


int main() {

  BLE &ble = BLE::Instance();

  Lab3ServerProcess ble_process(event_queue, ble);
  Lab3Server server;
  ble_process.on_init(callback(&server, &Lab3Server::start));
  ble_process.start();

  return 0;
}
