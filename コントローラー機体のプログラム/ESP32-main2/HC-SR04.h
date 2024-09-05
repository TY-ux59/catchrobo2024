const int TRIG1 = 27;
const int ECHO1 = 26;
const int TRIG2 = 17;
const int ECHO2 = 16;
double dist1 = 0;
double dist2 = 0;
const int MAX_WAIT = 20000;
double sonic = 331.5 + (27.0 * 0.6);  // 音速

void HCSR04_setup() {
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
}

double read_distance(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  double time = pulseIn(ECHO, HIGH, MAX_WAIT);
  double dist = (time * sonic * 100) / 1000000 / 2;
 
  return dist;
}
