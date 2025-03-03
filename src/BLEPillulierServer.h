#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

#define SERVICE_UUID "e85bec35-8807-48de-8269-d9c44052f6c9"
#define CHARACTERISTIC_UUID_PILS_TAKEN "1fec0cbe-51d0-49ed-b94f-75bd29b503d8"
#define BLE_SERVER_NAME "ESP32-Pilulier"

class PillulierServerCallback: public BLEServerCallbacks {
    public:
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);
};

// Callback for pillulier Characteristic write events
class PillulierCharacteristicCallbacks : public BLECharacteristicCallbacks {
    public:
        void onNotify(BLECharacteristic* pCharacteristic);
};

class BLEManager {
    public:
    void init_server_com();
    void BLESetValue(int val);
};