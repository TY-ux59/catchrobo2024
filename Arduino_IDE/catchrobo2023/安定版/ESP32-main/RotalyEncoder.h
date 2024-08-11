int pin1_encoder1 = 26;  //ロータリーエンコーダーのピン設定
int pin2_encoder1 = 27;
int pin1_encoder2 = 16;
int pin2_encoder2 = 17;
int pin1_encoder3 = 25;
int pin2_encoder3 = 35;
int pin1_encoder4 = 15;
int pin2_encoder4 = 21;
int valA = 0;
int valB = 0;
int valC = 0;
int valD = 0;
int valE = 0;
int valF = 0;
int valG = 0;
int valH = 0;
int preValA = 0;
int preValB = 0;
int preValC = 0;
int preValD = 0;
int preValE = 0;
int preValF = 0;
int preValG = 0;
int preValH = 0;
int rotation1 = 0;
int rotation2 = 0;
int rotation3 = 0;
int rotation4 = 0;
int preRotation1 = 0;
int difRotation1 = 0;
int preRotation2 = 0;
int difRotation2 = 0;
int preRotation3 = 0;
int difRotation3 = 0;
int preRotation4 = 0;
int difRotation4 = 0;
int goal1 = 60;  //ロータリーエンコーダーの目的となる値。調整する
int goal2 = 100;
int goal3 = 160;
int goal4 = 100;

int result1 = 0;
int result2 = 0;
int result3 = 0;
int result4 = 0;

int JudgeHighLow(int a) {
  if (a == HIGH) return 1;
  else return 0;
}

int motorEnd(int valA, int valB, int preValA, int preValB, int difRotation, int preRotation, int rotation) {
  difRotation = rotation - preRotation;
  // preValA = valA;
  // preValB = valB;
  preRotation = rotation;
  return 0;
}

int sumDif = 0;
int preDif = 0;
const float Vp = 0.3;  //PID制御で用いる値。調整する
const float Vi = 0.03;
const float Vd = 0.2;
int pid(int rotation, int goal) {  //PID制御の計算式
  float dif = goal - rotation;
  sumDif += dif;
  float DDif = dif - preDif;  //
  int result = dif * Vp + (sumDif / 100) * Vi + DDif * Vd;
  preDif = dif;
  // Serial.println(result);
  if (result >= 200) result = 200;
  return result;
}

int JudgeRotate(int a, int b, int preA, int preB) {  //ロータリーエンコーダーの回転方向を判別する
  bool high = false;
  bool low = false;
  if (preA == 0 && preB == 0) {
    if (a == 1 && b == 0) high = true;
    else if (a == 0 && b == 1) low = true;
  } else if (preA == 1 && preB == 0) {
    if (a == 1 && b == 1) high = true;
    else if (a == 0 && b == 0) low = true;
  } else if (preA == 1 && preB == 1) {
    if (a == 0 && b == 1) high = true;
    else if (a == 1 && b == 0) low = true;
  } else if (preA == 0 && preB == 1) {
    if (a == 0 && b == 0) high = true;
    else if (a == 1 && b == 1) low = true;
  }
  preA = a;
  preB = b;
  if (high == true && low == false) return 1;
  else if (high == false && low == true) return -1;
  else return 0;
}

int JudgeHigh(int a, int b, int preA, int preB) {  //上昇する値にのみ反応する
  //delay(5);
  bool flag = false;
  if (preA == 0 && preB == 0) {
    if (a == 1 && b == 0) flag = true;
    else flag = false;
  } else if (preA == 1 && preB == 0) {
    if (a == 1 && b == 1) flag = true;
    else flag = false;
  } else if (preA == 1 && preB == 1) {
    if (a == 0 && b == 1) flag = true;
    else flag = false;
  } else if (preA == 0 && preB == 1) {
    if (a == 0 && b == 0) flag = true;
    else flag = false;
  }
  preA = a;
  preB = b;
  if (flag == true) return 1;
  else return 0;
}
int JudgeLow(int a, int b, int preA, int preB) {  //下降する値にのみ反応する
  //delay(5);
  bool flag = false;
  if (preA == 0 && preB == 0) {
    if (a == 0 && b == 1) flag = true;
    else flag = false;
  } else if (preA == 1 && preB == 0) {
    if (a == 0 && b == 0) flag = true;
    else flag = false;
  } else if (preA == 1 && preB == 1) {
    if (a == 1 && b == 0) flag = true;
    else flag = false;
  } else if (preA == 0 && preB == 1) {
    if (a == 1 && b == 1) flag = true;
    else flag = false;
  }
  preA = a;
  preB = b;
  if (flag == true) return -1;
  else return 0;
}

int flagHigh = false;  //ロータリエンコーダの値の変化に一貫性を持たせるためのフラグ。
int flagLow = false;
int stopCount = 0;
int rotalyEncoder(int val1, int val2, int preVal1, int preVal2, int pin1, int pin2, int rotation) {  //ロータリーエンコーダーの挙動
  if (flagHigh == true && flagLow == false) {                                                        //上昇時は増加のみ、下降時は減少のみ
    rotation += JudgeHigh(val1, val2, preVal1, preVal2);                                             //"rotation"に値を加える
    flagLow = false;
  } else if (flagLow == true && flagHigh == false) {
    rotation += JudgeLow(val1, val2, preVal1, preVal2);
    flagHigh = false;
  } else if (flagHigh == false && flagLow == false) {  //フラグの点灯
    if (JudgeRotate(val1, val2, preVal1, preVal2) == 1) {
      flagHigh = true;
    } else if (JudgeRotate(val1, val2, preVal1, preVal2) == -1) {
      flagLow = true;
    }
  }
  if (flagHigh == true || flagLow == true) {
    if (preVal1 == val1 && preVal2 == val2) {  //一定時間停止していると上昇・下降フラグを切る。
      stopCount++;
      if (stopCount >= 1000) {
        preVal1 = 0;
        val1 = 0;
        preVal2 = 0;
        val2 = 0;
        flagHigh = false;
        flagLow = false;
        stopCount = 0;
      }
    } else stopCount = 0;
  }
  preVal1 = val1;
  preVal2 = val2;
  return rotation;
}

void RotalyEncoderSetup() {
  pinMode(pin1_encoder1, INPUT);
  pinMode(pin2_encoder1, INPUT);
  pinMode(pin1_encoder2, INPUT);
  pinMode(pin2_encoder2, INPUT);
  pinMode(pin1_encoder3, INPUT);
  pinMode(pin2_encoder3, INPUT);
  pinMode(pin1_encoder4, INPUT);
  pinMode(pin2_encoder4, INPUT);
  preValA = JudgeHighLow(digitalRead(pin1_encoder1));
  preValB = JudgeHighLow(digitalRead(pin2_encoder1));
  preValC = JudgeHighLow(digitalRead(pin1_encoder2));
  preValD = JudgeHighLow(digitalRead(pin2_encoder2));
  preValE = JudgeHighLow(digitalRead(pin1_encoder3));
  preValF = JudgeHighLow(digitalRead(pin2_encoder3));
  preValG = JudgeHighLow(digitalRead(pin1_encoder4));
  preValH = JudgeHighLow(digitalRead(pin2_encoder4));
}