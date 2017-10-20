#include "DHT.h"
#define DHTPIN 3     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

int fanSpeed = 0;
int commandCheck = 0;
String command;
char werd;
int fan = 11;
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(fan, OUTPUT);
  analogWrite(fan, 0);
  Serial.begin(9600);
  dht.begin();
  testFan();
  autoFan();
}

void testFan() {
  Serial.println("Fan Test");
  digitalWrite(led, HIGH);
  analogWrite(fan, 255);
  delay(4000);
  analogWrite(fan, fanSpeed);
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
      analogWrite(fan, fanSpeed);
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
    int t_old = 0;
    if (t != t_old){
      Serial.print(t);
      Serial.println(" *C");
    }
    t_old = t;
    if (t > 27){t = 27;}
    if (t < 19){t = 19;}
    
    fanSpeed = map(t, 19,27,0,255);
    analogWrite(fan, fanSpeed);
    Serial.println(fanSpeed);
    digitalWrite(led, LOW);
    delay(100);
  }
  Serial.println("Auto Mode Disabled");
}

void clear() {
  Serial.println();
}

