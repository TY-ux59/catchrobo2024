Servo servo1;
Servo servo2;
Servo servo3;
int servoPin1 = 25;
int servoPin2 = 26;
int servoPin3 = 27;
int pos1,pos2,pos3;
bool servoFlag1 = 0;
bool servoFlag2 = 0;

bool servoFlag = 0;

int minUs1 = 1000;
int maxUs1 = 2000;
int minUs2 = 10;
int maxUs2 = 3000;
int servoHz = 50;
int servo_interval = 5;
int pos1_setup = 0;
int pos2_setup = 180;
int pos3_setup = 100;

void Servo_setup() {
  servo1.setPeriodHertz(servoHz);  // Standard 50hz servo
  servo2.setPeriodHertz(servoHz);  // Standard 50hz servo
  servo3.setPeriodHertz(servoHz);  // Standard 50hz servo
  servo1.attach(servoPin1, minUs1, maxUs1);
  servo2.attach(servoPin2, minUs1, maxUs1);
  servo3.attach(servoPin3, minUs2, maxUs2);
  pos1 = pos1_setup;
  pos2 = pos2_setup;
  pos3 = pos3_setup;
  servo1.write(pos1);
  servo2.write(pos2);
  servo3.write(pos3);
}

void Servo_write() {
  servo1.write(pos1);
  servo2.write(pos2);
  servo3.write(pos3);
  Serial.print("Servo:");
  Serial.print(pos1);
  Serial.print("  |  ");
  Serial.print(pos2);
  Serial.print("  |  ");
  Serial.print(pos3);
  Serial.println();
}

void Servo_angle_range() {
  if (pos1 > 180) {
    pos1 = 180;
  } else if (pos1 < 0) {
    pos1 = 0;
  }

  if (pos2 > 180) {
    pos2 = 180;
  } else if (pos2 < 0) {
    pos2 = 0;
  }

  if (pos3 > 180) {
    pos3 = 180;
  } else if (pos3 < 0) {
    pos3 = 0;
  }
}
