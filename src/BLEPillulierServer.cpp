#include "BLEPillulierServer.h"
#include "DayModel.h"

bool deviceConnected = false;
BLEDescriptor configurationDescriptor(BLEUUID((uint16_t)0x2902));

DayModel dayInfos[7];

BLECharacteristic *pPillsCharacteristic;

void PillulierServerCallback::onConnect(BLEServer* pServer){
    deviceConnected = true;
}

void PillulierServerCallback::onDisconnect(BLEServer* pServer){
    if(deviceConnected){
        deviceConnected = false;
        esp_restart();
    }
}

void PillulierCharacteristicCallbacks::onNotify(BLECharacteristic *pCharacteristic) {
    if(!deviceConnected)
        return;
}

void HandleConfigChanged(std::string value){
    Serial.println("Received Value: ");
    Serial.println(value.c_str());

    ScheduleParser::parseDayPeriods(value.c_str(), dayInfos);
} 

void PillulierCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
    Serial.println("Message reçu");
    if(pCharacteristic->getUUID().equals(BLEUUID(CHARACTERISTIC_UUID_SET_CONFIG))){
        Serial.println("UUID Identique");
        std::string value = pCharacteristic->getValue();
        HandleConfigChanged(value);
    }
}

void BLEManager::init_server_com(){
    BLEDevice::init(BLE_SERVER_NAME);

    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new PillulierServerCallback());

    BLEService *pilsService = pServer->createService(SERVICE_UUID);

        // Create BLE Characteristic
    pPillsCharacteristic = pilsService->createCharacteristic(
        CHARACTERISTIC_UUID_PILS_TAKEN,
        BLECharacteristic::PROPERTY_READ | 
        BLECharacteristic::PROPERTY_NOTIFY
    );

    // Set Callback for Pillulier Characteristic
    pPillsCharacteristic->setCallbacks(new PillulierCharacteristicCallbacks());

    // Create BLE Descriptor (CCCD - Client Characteristic Configuration Descriptor)
    configurationDescriptor.setValue("Pils taken");
    pPillsCharacteristic->addDescriptor(&configurationDescriptor);

    //Config Characteristic
    BLECharacteristic *pConfigCharacteristic = pilsService->createCharacteristic(
        CHARACTERISTIC_UUID_SET_CONFIG,
        BLECharacteristic::PROPERTY_READ | 
        BLECharacteristic::PROPERTY_WRITE
    );

    // Set Callback for Config Characteristic
    pConfigCharacteristic->setCallbacks(new PillulierCharacteristicCallbacks());
    pConfigCharacteristic->setValue("{ \"TODO\": \"todo\" }");

    // Add Characteristic to Service
    pilsService->start();

    // Start BLE Advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    BLEDevice::startAdvertising();

    Serial.println("BLE LED Control Ready!");
    // put your setup code here, to run once:
}

void BLEManager::BLESetValue(int val){
    pPillsCharacteristic->setValue(val);
    pPillsCharacteristic->notify(true);
}