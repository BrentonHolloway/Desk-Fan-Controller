#include "DHT.h"
#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

int fanSpeed = 0;
int fanSpeed_old = 0;
int commandCheck = 0;
String command;
char werd;
int fan_MOSFET = 7;
int fan_PWM = 6;
int fan_RPM = 5;
int led = 13;
int t_old = 0;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(fan_MOSFET, OUTPUT);
  pinMode(fan_PWM, OUTPUT);
  pinMode(fan_RPM, INPUT);
  
  digitalWrite(fan_MOSFET, LOW);
  analogWrite(fan_PWM, 0);
  Serial.begin(9600);
  dht.begin();
  
  testFan();
  autoFan();
}

void testFan() {
  Serial.println("Fan Test");
  digitalWrite(led, HIGH);
  digitalWrite(fan_MOSFET, HIGH);
  analogWrite(fan_PWM, 255);
  delay(4000);
  analogWrite(fan_PWM, fanSpeed);
  digitalWrite(led, LOW);
  Serial.println("Fan Test Complete");
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
      testFan();
    } else if (command == "auto" ) {
      autoFan();
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
void autoFan() {
  Serial.println("Auto Mode Enabled");
  while (Serial.available() < 1) {
    digitalWrite(led, HIGH);
    int t = dht.readTemperature();
    if (t != t_old){
      Serial.print(t);
      Serial.println(" *C");
    }
    t_old = t;

    if(t < 19){
      digitalWrite(fan_MOSFET, LOW);
      fanSpeed = 0;
    }else if (t >= 19 && t <=27){
      digitalWrite(fan_MOSFET, LOW);
      fanSpeed = map(t, 19,27,150,255);
    }else if (t > 27){
      digitalWrite(fan_MOSFET, HIGH);
      fanSpeed = 255;
    }

    analogWrite(fan_PWM, fanSpeed);

    if (fanSpeed != fanSpeed_old){
      Serial.println(fanSpeed);
    }
    fanSpeed_old = fanSpeed;
    
    digitalWrite(led, LOW);
    delay(100);
  }
  Serial.println("Auto Mode Disabled");
}

void clear() {
  Serial.println();
}
