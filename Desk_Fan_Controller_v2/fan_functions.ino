#include "Arduino.h"

void testFan(int led, int fan_MOSFET, int fan_PWM, int fanSpeed) {
  Serial.println("Fan Test");
  digitalWrite(led, HIGH);
  digitalWrite(fan_MOSFET, HIGH);
  analogWrite(fan_PWM, 255);
  delay(4000);
  analogWrite(fan_PWM, fanSpeed);
  digitalWrite(led, LOW);
  Serial.println("Fan Test Complete");
}

void autoFan(int led, int fan_MOSFET, int fan_PWM, int fanSpeed) {
  static int fanSpeed_old = 0;
  
  Serial.println("Auto Mode Enabled");
  while (Serial.available() < 1) {
    digitalWrite(led, HIGH);
    int t = dht.readTemperature();
    print_temp(t, t_old);
    t_old = t;
    if(t < 19){
      digitalWrite(fan_MOSFET, LOW);
      fanSpeed = 0;
    }else if (t >= 19 && t <=27){
      digitalWrite(fan_MOSFET, HIGH);
      fanSpeed = map(t, 19,27,100,255);
    }else if (t > 27){
      digitalWrite(fan_MOSFET, HIGH);
      fanSpeed = 255;
    }

    analogWrite(fan_PWM, fanSpeed);
    
    print_speed(fanSpeed, fanSpeed_old);
    fanSpeed_old = fanSpeed;
    
    digitalWrite(led, LOW);
    delay(100);
  }
  Serial.println("Auto Mode Disabled");
}

void print_speed(int fanSpeed, int fanSpeed_old){
  if (fanSpeed != fanSpeed_old){
      Serial.println(fanSpeed);
    }
}

void print_temp(int temp, int old_temp){
  if (temp != old_temp){
      Serial.print(temp);
      Serial.println(" *C");
    }
}


