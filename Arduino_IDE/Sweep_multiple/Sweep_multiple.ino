#include <ESP32Servo.h>

Servo myservo1;
Servo myservo2;
Servo myservo3;

//4,5,25,26,27で動作確認、16,17は動作しない
const int servoPin1 = 16;
const int servoPin2 = 17;
const int servoPin3 = 27;

int pos = 0;

void setup() {
	myservo1.setPeriodHertz(50);
	myservo1.attach(servoPin1, 1000, 2000);
  myservo2.setPeriodHertz(50);
	myservo2.attach(servoPin2, 1000, 2000);
  myservo3.setPeriodHertz(50);
	myservo3.attach(servoPin3, 1000, 2000);
}

void loop() {

	for (pos = 0; pos <= 180; pos += 1) {
		// in steps of 1 degree
		myservo1.write(pos);
    myservo2.write(pos);
    myservo3.write(pos);
		delay(15);
	}
	for (pos = 180; pos >= 0; pos -= 1) {
		myservo1.write(pos);
    myservo2.write(pos);
    myservo3.write(pos);
		delay(15);
	}
}

