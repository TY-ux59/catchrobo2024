

#include <ESP32Servo.h>
#include <PS4Controller.h>
#include "MotorDriver.h"
#include <ESP32Servo.h>
#include <stdio.h>

int angleA = 0;
int move2 = 0;

int preMillis = 0;
bool LStickFlag = false;
int LStickX = 0;
int preLStickX = 0;
char can_receive = ' ';
char pre_can_receive = ' ';
char pre_can_send = ' ';
bool constState = false;
String button = " ";
String preButton = " ";

Servo myservo1;  // create servo object to control a servo
Servo myservo2;
Servo myservo3;
// 16 servo objects can be created on the ESP32

char message[50];

int pos = 0;  // variable to store the servo position
ESP32PWM pwm;
int servoPin1 = 26;
int servoPin2 = 27;
int servoPin3 = 25;

int servoHz = 50;
int minUs1 = 1000;
int maxUs1 = 2000;

int volume = minUs1;

//仕分けノモーターの角度の変数
int M2DefaultAngle = 1;
int M2EbiAngle = 30;
int M2YuzuAngle = 60;
int M2NoriAngle = 70;

//そうじ機ノモーターの角度の変数
int M3CloseAngle = 1;
int M3OpenAngle = 10;

void setup() {
  Motor_stop();
  Serial.begin(115200);
  PS4.begin();  //write PS4 id Macアドレスを書き込む
  Serial.println("Ready.");

  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo1.setPeriodHertz(50);  // standard 50 hz servo
  myservo2.setPeriodHertz(50);
  myservo3.setPeriodHertz(50);
  myservo1.attach(servoPin1, 1000, 2000);  // attaches the servo on pin 18 to the servo object
  myservo2.attach(servoPin2, 1000, 2000);
  myservo3.attach(servoPin3, 1000, 2000);

  esc_1.setPeriodHertz(servoHz);          // Standard 50hz servo
  esc_1.attach(escPin1, minUs1, maxUs1);  //ESCへの出力ピンをアタッチします
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
  Serial.println("Writing minimum output");
  esc_1.writeMicroseconds(minUs1);  //ESCへ最小のパルス幅を指示します

  Serial.println("Wait 8 seconds. Then motor starts");
  delay(8000);

  MotorDriver_setup();
  preMillis = millis();
}

void loop() {

  for (int pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo1.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);            // waits 15ms for the servo to reach the position
  }
  for (int pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo2.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);            // waits 15ms for the servo to reach the position
  }
  for (int pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo3.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);            // waits 15ms for the servo to reach the position
  }
}

void PS4_control() {
  //PS4のコントローラ用
  //機体を取る部分を上げ下げする
  if (PS4.Up()) {  //motor2 forward.
                   //Serial.println("L1 Button");
    digitalWrite(PWM2, HIGH);

    digitalWrite(DIR2, LOW);
  } else if (PS4.Down()) {  //motor2 backward.
    //Serial.println("L2 Button");
    digitalWrite(PWM2, HIGH);

    digitalWrite(DIR2, HIGH);
    //棒を前後
  } else if (PS4.L2()) {  //motor3 up.
    //Serial.println("R1 Button");
    digitalWrite(PWM3, HIGH);
    digitalWrite(DIR3, LOW);
  } else if (PS4.R2()) {  //motor down.
    //Serial.println("R2 Button");
    digitalWrite(PWM3, HIGH);
    digitalWrite(DIR3, HIGH);

  } else if (PS4.Cross()) {
    Serial.println("CrossButton");
    while (volume < 1300) {
      volume += 5;
      Serial.println(volume);
    }
    sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
    Serial.println(message);                                //可変抵抗の値をシリアルモニタに表示
    esc_1.writeMicroseconds(volume);                        // パルス幅 `volume` のPWM信号を送信する
    preButton = "Cross";
  } else if (PS4.Circle()) {
    button = "Circle";
    if (button != preButton) {
      Serial.println(button);
      //Ebiを取るときに開けっぱなしにする
      //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
      //constStateは開けっ放しかどうかtrueであきっぱなし
      if (constState == false) {
        Serial.println("constOpen");
        //開いている状態を維持するかどうか
        servo3.write(M3OpenAngle);
        delay(100);
        constState = true;
        servo2.write(M2EbiAngle);
        delay(250);

      } else {
        Serial.println("ret");
        servo2.write(M2DefaultAngle);
        constState = false;
      }
      preButton = button;
    }


  } else if (PS4.Square()) {
    button = "Square";
    if (button != preButton) {
      Serial.println(button);
      //Yuzuを取るときに開けっぱなしにする
      //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
      if (constState == false) {
        Serial.println("constOpen");
        servo3.write(M3OpenAngle);
        constState = true;
        servo2.write(M2YuzuAngle);
      } else {
        Serial.println("ret");
        servo2.write(M2DefaultAngle);
        constState = false;
      }
      preButton = button;
    }
  } else if (PS4.Triangle()) {
    button = "Triangle";
    if (button != preButton) {
      Serial.println(button);
      //Noriを取るときに開けっぱなしにする
      //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
      if (constState == false) {
        Serial.println("constOpen");
        servo3.write(M3OpenAngle);
        constState = true;
        servo2.write(M2NoriAngle);
      } else {
        Serial.println("ret");
        servo2.write(M2DefaultAngle);
        constState = false;
      }
      preButton = button;
    }

  } else {
    Serial.println("n");
    volume = 1000;
    esc_1.writeMicroseconds(volume);
  }

  //ラズベリーパイとのシリアル通信
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');

    if (receivedMessage == "M3close") {
      servo3.write(M3CloseAngle);
      delay(100);
    }
    if (receivedMessage == "M3Open") {
      servo3.write(M3CloseAngle);
      delay(100);
    }

    if (receivedMessage == "Ebi") {
      servo2.write(M2EbiAngle);
      delay(100);
      servo2.write(M2DefaultAngle);
      delay(100);
    } else if (receivedMessage == "Yuzu") {
      servo2.write(M2YuzuAngle);
      delay(100);
      servo2.write(M2DefaultAngle);
      delay(100);
    } else if (receivedMessage == "Nori") {
      servo2.write(M2NoriAngle);
      delay(100);
      servo2.write(M2DefaultAngle);
      delay(100);
    }
  }

  //Serial.printf("Right Stick y at %d\n", PS4.RStickY());
  //スティックの傾きに合わせて、回転速度を調整
  //Lスティックの反応が悪かったため、Rスティックに変更
  int RStickY = map(PS4.RStickY(), -128, 128, -256, 256);
  if (RStickY <= 20 && RStickY >= -20) {
    ledcWrite(0, 0);
  } else {
    if (can_receive == 'x') {
      digitalWrite(PWM4, LOW);
    } else {
      ledcWrite(0, abs(RStickY));
    }
  }
  //回転方向
  if (RStickY >= 0) {  //left
    digitalWrite(DIR4, HIGH);
  } else {  ///right
    digitalWrite(DIR4, LOW);
  }

  LStickX = map(PS4.LStickX(), -128, 128, -256, 256);
  if (LStickX <= 20 && LStickX >= -20) {
    if (LStickFlag == true) {
      int nowMillis = millis();
      if (nowMillis - preMillis >= 100) {
        if (preLStickX >= 20) {
          preLStickX -= 10;
          ledcWrite(1, abs(preLStickX));
        } else if (LStickX <= -20) {
          preLStickX += 10;
          ledcWrite(1, abs(preLStickX));
        } else {
          preLStickX = 0;
          LStickFlag = false;
          ledcWrite(1, 0);
        }
        preMillis = nowMillis;
        Serial.println(LStickX);
      }
    }
  } else {
    ledcWrite(1, abs(LStickX));
    LStickFlag = true;
    preLStickX = LStickX;
  }
  //回転方向
  if (LStickX >= 0) {  //back
    digitalWrite(DIR1, LOW);
  } else {  //forward
    digitalWrite(DIR1, HIGH);
  }
}
