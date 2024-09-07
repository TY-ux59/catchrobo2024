#include <stdio.h>
#include <Servo.h>
#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);
  Servo servo;
  servo.attach(26);
  servo.write(45);
}

void loop() {

  PS4_control();

  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');
    Serial.print("Received from PC: ");
    Serial.println(receivedMessage);

    if (receivedMessage == "M2close") {
      servo.write(-45);
      delay(250);
    }
    if (receivedMessage == "M2Open") {
      servo.write(45);
      delay(250);
    }
  }
}

void PS4_control() {
  servo.write(45);
  delay(250);
  bool run = true;
  if (PS4.Circle()) {
    //Ebiを取るときに開けっぱなしにする
    //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
    Serial.println("constOpen");
    //モーター（仕分け）に指示
    Serial.println("EbiM1");
    while (run) {
      if (PS4.Circle()) {
        Serial.println("ret");
        run = false;
      }
    }
  } else if (PS4.Square()) {
    //Yuzuを取るときに開けっぱなしにする
    //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
    Serial.println("constOpen");
    //モーター（仕分け）に指示
    Serial.println("YuzuM1");
    while (run) {
      if (PS4.Square()) {
        Serial.println("ret");
        run = false;
      }
    }
  } else if (PS4.Triangle()) {
    //Noriを取るときに開けっぱなしにする
    //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
    Serial.println("constOpen");
    //モーター（仕分け）に指示
    Serial.println("NoriM1");
    while (run) {
      if (PS4.Triangle()) {
        delay(250);
        run = false;
      }
    }
  }
}
