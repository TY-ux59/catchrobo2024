void setup(){
  Serial.begin(115200);
  Serial1.begin(19200, SERIAL_8N1, 33, 32);
}

void loop() {
  if (Serial.available() > 0) {
    String receivedMessage = Serial.readStringUntil('\n');
    Serial.print("Received from PC: ");
    Serial.println(receivedMessage);
  }
}