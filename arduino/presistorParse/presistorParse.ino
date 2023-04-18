// Declare global variables
const int dimH = 3;
const int dimV = 3;
int pinMXEN = 1;
int pinMXA0 = 2;
int pinMXA1 = 3;
int pinMXA2 = 4;
int pinAnalogIn = A0;
int mxTT[9] = { 0000, 1000, 1100, 1010, 1110, 1001, 1101, 1011, 1111 };
double ambTHold = 0;
double analogOutputs[dimH + dimV];
bool pinState[dimH+dimV];

void setup() {

  // Set pin modes for multiplexer control pins
  pinMode(pinMXEN, OUTPUT);
  pinMode(pinMXA0, OUTPUT);
  pinMode(pinMXA1, OUTPUT);
  pinMode(pinMXA2, OUTPUT);
  Serial.begin(9600);

  // Set pin mode for analog input pin
  pinMode(pinAnalogIn, INPUT);
}

//Converts the target photoresistor into the high/low boolean of a specific output pin to multiplexer
bool mxGetPR(int tgtR, int tt[9], String pin, int size) {
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

//Average double values sequentially through an array (used for ambient light threshold)
double avgAmbTHold(int arrSize, double *valuesIn) {
  double sum = 0;
  for (int i = 0; i < arrSize; i++) {
    sum = sum+valuesIn[i];
  }
  return sum/arrSize;
}

void loop() {
  //Get averaged ambient light value for comparison
  for (int i = 0; i < (dimH + dimV); i++) {
    //Set pinModes according to the index of the photoresistor
    pinMode(pinMXEN, mxGetPR(i, mxTT, "EN", dimH + dimV));  
    pinMode(pinMXA0, mxGetPR(i, mxTT, "A0", dimH + dimV));  
    pinMode(pinMXA1, mxGetPR(i, mxTT, "A1", dimH + dimV));  
    pinMode(pinMXA2, mxGetPR(i, mxTT, "A2", dimH + dimV));  
    delayMicroseconds(100);
    analogOutputs[i] = analogRead(pinAnalogIn);
  }
  ambTHold = avgAmbTHold(dimH+dimV, analogOutputs); // Use dimH+dimV instead of sizeof(analogOutputs)/sizeof(analogOutputs[0])
  // Check analog signal of all photoresistors
  for (int j = 0; j < (sizeof(analogOutputs)/sizeof(int)); j++ ) {
    pinMode(pinMXEN, mxGetPR(j, mxTT, "EN", dimH + dimV));  
    pinMode(pinMXA0, mxGetPR(j, mxTT, "A0", dimH + dimV));  
    pinMode(pinMXA1, mxGetPR(j, mxTT, "A1", dimH + dimV));  
    pinMode(pinMXA2, mxGetPR(j, mxTT, "A2", dimH + dimV));  
    delayMicroseconds(1000);
    pinState[j] = (analogRead(pinAnalogIn) > ambTHold);
  }
  for (int k = 0; k < (sizeof(pinState)/sizeof(bool)); k++) {
    Serial.write(pinState[k]);
  }
  Serial.write("Pin Read Cycle Complete");
}
