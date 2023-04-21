const int dimH = 3;
const int dimV = 3;
int pinMXEN = 0;
int pinMXA0 = 1;
int pinMXA1 = 2;
int pinMXA2 = 3;
int pinAnalogIn = A0;
int mxTT[9] = { 0000, 1000, 1100, 1010, 1110, 1001, 1011, 1101, 1111 };
double ambTHold = 0;
double analogOutputs[dimH + dimV];
bool pinState[dimH+dimV];

void setup() {
  // put your setup code here, to run once:
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(0, OUTPUT);
  Serial.begin(9600);
}

bool mxGetPR(int tgtR, int tt[9], String pin) {
  //Output pin high/low in the order EN, A0, A1, A2
  int pinStates[4] = { tt[tgtR] / 1000, (tt[tgtR] / 100) % 10, (tt[tgtR] / 10) % 10, tt[tgtR] % 10 };
  //Return booleans according to the pinStates array, return false as default
  if (pin == "EN") {
    return static_cast<bool>(pinStates[0]);
  } else if (pin == "A0") {
    return static_cast<bool>(pinStates[1]);
  } else if (pin == "A1") {
    return static_cast<bool>(pinStates[2]);
  } else if (pin == "A2") {
    return static_cast<bool>(pinStates[3]);
  } else {
    return false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 9; i++) {
    Serial.print(mxGetPR(i, mxTT, "EN"));
    Serial.print(mxGetPR(i, mxTT, "A0"));
    Serial.print(mxGetPR(i, mxTT, "A1"));
    Serial.print(mxGetPR(i, mxTT, "A2"));
    Serial.println(i);
    Serial.println();
    digitalWrite(pinMXEN, mxGetPR(i, mxTT, "EN"));  
    digitalWrite(pinMXA0, mxGetPR(i, mxTT, "A0"));  
    digitalWrite(pinMXA1, mxGetPR(i, mxTT, "A1"));  
    digitalWrite(pinMXA2, mxGetPR(i, mxTT, "A2")); 
    delay(2000);     
  }
}
