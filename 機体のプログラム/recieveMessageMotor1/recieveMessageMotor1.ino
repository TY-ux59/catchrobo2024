#include <stdio.h>
#include <Servo.h>
//サーボモーターを制御するためのインスタンスを作成
void setup() {
  Serial.begin(115200);
  Servo servo;
  servo.attach(25);  //サーボモーターの制御信号ピンを指定
  const String match1 = "M1";
  const String match2 = "constOpen"
  servo.write(0);
}

void loop() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');
    Serial.print("Received from PC: ");
    Serial.println(receivedMessage);

    if (strstr(receivedMessage, match1) != NULL) {
      changeMotorAngleTemp(receivedMessage);
    }
    if (strstr(receivedMessage, match2) != NULL) {
      changeMotorAngleConst(receivedMessage);
    }
  }
}

void changeMotorAngleTemp(receivedMessage) {
  if (receivedMessage == "EbiM1") {
    servo.write(30);
    delay(250);
    servo.write(-30);
    delay(250);
  } else if (receivedMessage == "YuzuM1") {
    servo.write(60);
    delay(250);
    servo.write(-60);
    delay(250);
  } else {
    servo.write(90);
    delay(250);
    servo.write(-90);
    delay(250);
  }
}

void changeMotorAngleConst() {
  //
  bool run = true;
  if (receivedMessage == "EbiM1") {
    servo.write(30);
    delay(250);
    while (run) {
      run = returnCheck();
    }
    servo.write(-30);
    delay(250);
  } else if (receivedMessage == "YuzuM1") {
    servo.write(60);
    delay(250);
        while (run) {
      run = returnCheck();
    }
    servo.write(-60);
    delay(250);
  } else {
    servo.write(90);
    delay(250);
        while (run) {
      run = returnCheck();
    }
    servo.write(-90);
    delay(250);
  }
}

//コントローラーから戻すように指示が来ているかどうかを監視する
bool returnCheck() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');
    Serial.print("Received from PC: ");
    Serial.println(receivedMessage);
  }
  if (receivedMessage == "ret"){
    return false;
  }else{
    return true;
  }
}
