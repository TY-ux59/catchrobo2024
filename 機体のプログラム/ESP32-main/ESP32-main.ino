//PS4Controller.hを使用するため、ESP32のバージョンは1.0.6推奨
//PS4Controller.hに関するページ（環境構築）：https://404background.com/program/esp32-dualshock4/
//PS4Controller.hに関するページ（修正後ライブラリ）：https://404background.com/program/esp32-dualshock4-library/
//CAN.hのライブラリ：https://github.com/sandeepmistry/arduino-CAN
//PS4Controller.hのライブラリ：https://github.com/404background/PS4-esp32

#include <PS4Controller.h>
#include "MotorDriver.h"
#include <ESP32Servo.h>

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

//サーボ諸々
const int escPin1 = 25;
const int servoPin2 = 26;
const int servoPin3 = 27;
const int servoPin4 = 4;

char message[50];  //シリアルモニタへ表示する文字列を入れる変数

Servo esc_1;
Servo servo2;  //仕分けのモーター
Servo servo3;  //そうじ機のモーター
Servo servo4;  //仕分け機移動のモーター

int servoHz = 50;
int minUs1 = 1000;
int maxUs1 = 2000;

float volume = 1000;

//仕分けノモーターの角度のtrueの時の角度
//bool M2isLeft = true;
int M2DefaultAngle = 172;
int M2EbiAngle = 80;
int M2YuzuAngle = 135;
int M2NoriAngle = 127;

//そうじ機ノモーターの角度の変数
int M3CloseAngle = 18;
int M3OpenAngle = 45;
//サーボ4は360度回転で90で停止する
int servo4Angle = 90;

//ここを追加！！！！！！！！！！！！！！！！！！！！！
bool raspiConnection = true;

void setup() {
  Motor_stop();
  MotorDriver_setup();
  Serial.begin(115200);
  PS4.begin();
  Serial.println("Ready.");

  esc_1.setPeriodHertz(servoHz);
  servo2.setPeriodHertz(servoHz);
  servo3.setPeriodHertz(servoHz);
  servo4.setPeriodHertz(servoHz);
  esc_1.attach(escPin1, minUs1, maxUs1);
  servo2.attach(servoPin2, 500, 2400);
  servo3.attach(servoPin3, 500, 2400);
  servo4.attach(servoPin4, 500, 2500);

  Serial.println("Writing minimum output");
  servo2.write(M2DefaultAngle);
  servo3.write(M3CloseAngle);
  servo4.write(90);


  //ESCの設定
  esc_1.writeMicroseconds(minUs1);
  Serial.println("Wait 8 seconds. Then motor starts");
  delay(8000);
  preMillis = millis();
}
void loop() {
  PS4_control();
  Motor_stop();
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
  } else if (PS4.Right()) {  //motor2 backward.
    servo4Angle += 5;
    servo4.write(servo4Angle);
    delay(20);
  } else if (PS4.Left()) {
    servo4Angle -= 5;
    servo4.write(servo4Angle);
    delay(20);
  } else if (PS4.L2()) {  //motor3 up.
    //Serial.println("R1 Button");
    digitalWrite(PWM3, HIGH);
    digitalWrite(DIR3, LOW);
  } else if (PS4.R2()) {  //motor down.
    //Serial.println("R2 Button");
    digitalWrite(PWM3, HIGH);
    digitalWrite(DIR3, HIGH);

  } else if (PS4.Cross()) {
    servo3.write(M3CloseAngle);
    Serial.println("CrossButton");
    while (volume < 1700) {
      volume += 0.2;
      //Serial.println(volume);
    }
    //sprintf(message, "Pulse Width: %d micro sec", volume);  //シリアルモニタに表示するメッセージを作成
    //Serial.println(message);                                //可変抵抗の値をシリアルモニタに表示
    esc_1.writeMicroseconds(volume);  // パルス幅 `volume` のPWM信号を送信する
    preButton = "Cross";
  } else if (PS4.Circle()) {
    button = "Circle";
    if (button != preButton) {
      //次の行のprintlnが実行されるとconstOpenが実行されない
      //Serial.println(button);
      //Ebiを取るときに開けっぱなしにする
      //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
      //constStateは開けっ放しかどうかtrueであきっぱなし
      if (constState == false) {
        Serial.println("constOpen");

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
      //次の行のprintlnが実行されるとconstOpenが実行されない
      //Serial.println(button);
      //Yuzuを取るときに開けっぱなしにする
      //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
      if (constState == false) {
        Serial.println("constOpen");
        constState = true;
        servo2.write(M2YuzuAngle);
        delay(250);

      } else {
        Serial.println("ret");
        servo2.write(M2DefaultAngle);
        delay(250);
        constState = false;
      }
      preButton = button;
    }
  } else if (PS4.Triangle()) {
    button = "Triangle";
    if (button != preButton) {
      //次の行のprintlnが実行されるとconstOpenが実行されない
      //Serial.println(button);
      //Noriを取るときに開けっぱなしにする
      //仕分け部分のモーターと吸う部分のモーターを開けっぱなしにする
      if (constState == false) {

        Serial.println("constOpen");
        constState = true;
        servo2.write(M2NoriAngle);
        delay(250);
      } else {
        Serial.println("ret");
        servo2.write(M2DefaultAngle);
        delay(250);
        constState = false;

        preButton = button;
      }
    }


  } else if (PS4.Options()) {
    button = "Options";
    if (button != preButton) {
      constState = !constState;
      servo3.write(M3OpenAngle);
      delay(100);
      servo2.write(M2DefaultAngle);
      delay(100);
      Serial.println()
    }
    preButton = button;
  } else if (PS4.Share()) {
    //間違えてshareボタンをおしてしまった場合を想定
    button = "Share";
    if (button != preButton) {
      if (raspiConnection == true) {
        raspiConnection = false;
      } else {
        raspiConnection = true;
      }
    }
    preButton = button;
  } else {
    //90で停止
    servo4Angle = 90;
    servo4.write(servo4Angle);
    //Serial.println("n");
    volume = 1000;
    esc_1.writeMicroseconds(volume);
  }


  //ラズベリーパイとのシリアル通信
  if (Serial.available() > 0 && raspiConnection == true) {

    int receivedMessage = Serial.read();

    if (receivedMessage == 120) {
      servo3.write(M3CloseAngle);
      delay(100);
    }

    //吸う時に閉じる必要ができたので必要ない
    /*
    if (receivedMessage == "M3Open") {
      servo3.write(M3CloseAngle);
      delay(100);
    }*/

    if (receivedMessage == 101) {
      //Serial.write("ebi");
      servo2.write(M2EbiAngle);
      delay(100);
    } else if (receivedMessage == 121) {
      //Serial.write("yuzu");
      servo2.write(M2YuzuAngle);
      delay(100);
    } else if (receivedMessage == 110) {
      servo2.write(M2NoriAngle);
      delay(100);
    }
  }

  //Serial.printf("Right Stick y at %d\n", PS4.RStickY());
  //スティックの傾きに合わせて、回転速度を調整
  //Lスティックの反応が悪かったため、Rスティックに変更
  int RStickY = map(PS4.RStickY(), -128, 128, -256, 256);
  if (RStickY <= 20 && RStickY >= -20) {
    ledcWrite(ledcChannel1, 0);
  } else {
    ledcWrite(ledcChannel1, abs(RStickY));
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
          ledcWrite(ledcChannel2, abs(preLStickX));
        } else if (LStickX <= -20) {
          preLStickX += 10;
          ledcWrite(ledcChannel2, abs(preLStickX));
        } else {
          preLStickX = 0;
          LStickFlag = false;
          ledcWrite(ledcChannel2, 0);
        }
        preMillis = nowMillis;
        Serial.println(LStickX);
      }
    }
  } else {
    ledcWrite(ledcChannel2, abs(LStickX));
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
