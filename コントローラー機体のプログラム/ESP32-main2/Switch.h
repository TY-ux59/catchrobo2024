int switch1 = 12;
int switch2 = 18;
int switch3 = 32;

void SwitchSetup() {
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
}

bool SwitchStates(int pin) {
  bool status = digitalRead(pin);
  return status;
}
