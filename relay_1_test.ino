#include <BLEDevice.h>
#include <BLEServer.h>

const int RELAIS = 23;
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class Commande : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *c) {
    String v = c->getValue();
    if (v == "1") digitalWrite(RELAIS, LOW);   // rampe allumée
    if (v == "0") digitalWrite(RELAIS, HIGH);  // rampe éteinte
  }
};

class Connexion : public BLEServerCallbacks {
  void onDisconnect(BLEServer *s) { BLEDevice::startAdvertising(); }
};

void setup() {
  pinMode(RELAIS, OUTPUT);
  digitalWrite(RELAIS, HIGH);  // relais au repos au démarrage

  BLEDevice::init("Rampe");
  BLEServer *serveur = BLEDevice::createServer();
  serveur->setCallbacks(new Connexion());
  BLEService *service = serveur->createService(SERVICE_UUID);
  BLECharacteristic *ch = service->createCharacteristic(
      CHAR_UUID, BLECharacteristic::PROPERTY_WRITE);
  ch->setCallbacks(new Commande());
  service->start();
  BLEDevice::getAdvertising()->addServiceUUID(SERVICE_UUID);
  BLEDevice::startAdvertising();
}

void loop() {}