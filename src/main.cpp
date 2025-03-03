#include "BLEPillulierServer.h"
#include "HX711.h"
#include "time.h"

// HX711 circuit wir
#define LOADCELL_DOUT_PIN 2 
#define LOADCELL_SCK_PIN 3
#define TARE_BUTTON 6

#define LED_LUNDI 1
#define LED_MARDI 4
#define LED_MERCREDI 5
#define LED_JEUDI 9
#define LED_VENDREDI 8
#define LED_SAMEDI 7
#define LED_DIMANCHE 44

HX711 scale;
BLEManager bleManager;

volatile bool isTareBtnPressed = false;
float oldWeight = 0;

void Tare(){
  if(scale.is_ready()){
    Serial.println("Tare...");
    delay(1000);
    scale.tare();
    Serial.println("Tare done...");
  }
}

bool detect_change_weight(){
  float current_weight = std::floor(scale.get_units(10));
  bool isWeightDiff = oldWeight != current_weight;

  if(isWeightDiff){
    Serial.println("Changement de poids");
    oldWeight = current_weight;
  }

  bleManager.BLESetValue(isWeightDiff);
  return isWeightDiff;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(TARE_BUTTON, INPUT_PULLUP);

  pinMode(LED_LUNDI, OUTPUT);
  pinMode(LED_MARDI, OUTPUT);
  pinMode(LED_MERCREDI, OUTPUT);
  pinMode(LED_JEUDI, OUTPUT);
  pinMode(LED_VENDREDI, OUTPUT);
  pinMode(LED_SAMEDI, OUTPUT);
  pinMode(LED_DIMANCHE, OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale(712.66);
  scale.tare();

  digitalWrite(LED_LUNDI, LOW);
  digitalWrite(LED_MARDI, LOW);
  digitalWrite(LED_MERCREDI, LOW);
  digitalWrite(LED_JEUDI, LOW);
  digitalWrite(LED_VENDREDI, LOW);
  digitalWrite(LED_SAMEDI, LOW);
  digitalWrite(LED_DIMANCHE, LOW);

  bleManager.init_server_com();
}

void loop() {
  isTareBtnPressed = !digitalRead(TARE_BUTTON);

  if(isTareBtnPressed){
    Tare();
    isTareBtnPressed = false;
  }

  if (scale.is_ready()) {

    Serial.println("Place a known weight on the scale...");
    delay(1000);

    Serial.print("one reading:\t");
    Serial.print(scale.get_units(), 1);
    Serial.print(" g");
    Serial.print("\t| average:\t");
    Serial.print(std::floor(scale.get_units(10)), 2);
    Serial.print(" g \n");

    detect_change_weight();
  } 
  else {
    Serial.println("HX711 not found.");
  }

  delay(500);
}

//calibration factor will be the (reading)/(known weight)