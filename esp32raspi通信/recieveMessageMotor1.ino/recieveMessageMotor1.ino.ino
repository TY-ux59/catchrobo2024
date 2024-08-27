#include <Servo.h>
Servo servo;  //サーボモーターを制御するためのインスタンスを作成
void setup() {
  Serial.begin(115200);
  servo.attach(9);  //サーボモーターの制御信号ピンを指定
}

void loop() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');
    Serial.print("Received from PC: ");
    Serial.println(receivedMessage); 
  }
  const char* match = "motor1";
  if (strstr(receivedMessage, match) != NULL) {
    changeMotorAngle(receivedMessage);
  }
}

void changeMotorAngle() {
}
