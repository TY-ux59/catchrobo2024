+#include <stdio.h>
#include <Servo.h>

void setup() {
  Serial.begin(115200);
  Servo servo;
  servo.attach(9);
  servo.write(45);
}

void loop() {
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
    if (receivedMessage == "constOpen") {
      servo.write(45);
      delay(250);
      run = true;
      while (run) {
        run = returnCheck();
      }
    }
  }
}

//コントローラーから戻すように指示が来ているかどうかを監視する
bool returnCheck() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');
    Serial.print("Received from PC: ");
    Serial.println(receivedMessage);
  }
  if (receivedMessage == "ret") {
    return false;
  } else {
    return true;
  }
}
