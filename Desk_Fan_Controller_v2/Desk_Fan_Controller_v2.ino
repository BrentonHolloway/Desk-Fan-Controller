#include "DHT.h"
#include "fan_functions.h"
#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

int fanSpeed = 0;
int commandCheck = 0;
String command;
char werd;
int fan_MOSFET = 7;
int fan_PWM = 6;
int fan_RPM = 5;
int led = 13;
int t_old = 0;

void setup() {
  
  pinMode(fan_PWM, OUTPUT);
  
  pinMode(led, OUTPUT);
  pinMode(fan_MOSFET, OUTPUT);
  pinMode(fan_RPM, INPUT);

  digitalWrite(fan_MOSFET, LOW);
  analogWrite(fan_PWM, 0);

  dht.begin();
  Serial.begin(9600);

  testFan(led, fan_MOSFET, fan_PWM, fanSpeed);
  autoFan(led, fan_MOSFET, fan_PWM, fanSpeed);
}

void loop() {
  while ( Serial.available() > 0 ) {
    delay(5);
    werd = Serial.read();
    command += werd;
  }
  Serial.flush();
  if ( command.length() > 0 ) {
    if ( command == "test" ) {
      testFan(led, fan_MOSFET, fan_PWM, fanSpeed);
    } else if (command == "auto" ) {
      autoFan(led, fan_MOSFET, fan_PWM, fanSpeed);
    } else {
      commandCheck = 1;
    }
    if ( commandCheck == 1 ) {
      fanSpeed = command.toInt();
      analogWrite(fan_PWM, fanSpeed);
      Serial.print("Fan Speed: ");
      Serial.println(fanSpeed);
      commandCheck = 0;
    }
    command = "";
    clear();
  }
}

void clear() {
  Serial.println();
}
