int switch1;
int switch2;

bool push = false;

void stopButton() {
  if (push == true) {
    digitalWrite(PWM1, LOW);
    digitalWrite(PWM2, LOW);
    digitalWrite(PWM3, LOW);
    digitalWrite(PWM4, LOW);
  }
}