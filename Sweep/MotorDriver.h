const int PWM1 = 12;  //PWM availible on: 2,4,5,12-19,21-23,25-27,32-33
const int DIR1 = 14;
const int PWM2 = 18;
const int DIR2 = 19;
const int PWM3 = 23;
const int DIR3 = 22;
const int PWM4 = 32;
const int DIR4 = 33;

void MotorDriver_setup() {
  pinMode(PWM1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM3, OUTPUT);
  pinMode(DIR3, OUTPUT);
  pinMode(PWM4, OUTPUT);
  pinMode(DIR4, OUTPUT);
  ledcSetup(0, 12800, 8);
  ledcAttachPin(PWM4, 0);
  ledcSetup(1, 12800, 8);
  ledcAttachPin(PWM1, 1);
}

void Motor_stop() {
  digitalWrite(PWM1, LOW);
  digitalWrite(PWM2, LOW);
  digitalWrite(PWM3, LOW);
  digitalWrite(PWM4, LOW);
}