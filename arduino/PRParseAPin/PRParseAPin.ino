// Declare global variables
const int dimH = 2;
const int dimV = 2;
int prAnalogIn[]={A0,A1,A2,A3,A4,A5,A6};
int pinReset = 0;
double ambThreshold[dimH+dimV];
double analogOutputs[dimH + dimV];
double laserDiff[]={100,20,100,100};
bool pinState[dimH+dimV];

//Average double values sequentially through an array (used for ambient light threshold)
double avgAmbTHold(int arrSize, double *valuesIn) {
  double sum = 0;
  for (int i = 0; i < arrSize; i++) {
    sum = sum+valuesIn[i];
  }
  return sum/arrSize;
}

void setup() {
  // Set pin modes for analog output pins
  for (int i = 0; i < dimH+dimV; i++) {
    pinMode(prAnalogIn[i], INPUT);
  }
  pinMode(pinReset, INPUT); // Set pin mode for button reset pin
  digitalWrite(pinReset, LOW);
  //Start serial output
  Serial.begin(9600);
  //Identify ambient light level on startup
  for (int i = 0; i < (dimH + dimV); i++) {
    //Read all analog pins
    delayMicroseconds(100);
    ambThreshold[i] = analogRead(prAnalogIn[i]);
  }
}

void loop() {
  
  // Check analog signal of all photoresistors if reset button is hit
  if (digitalRead(pinReset) == HIGH) {
    Serial.print("RESET");
    Serial.println();
  //Identify ambient light level on startup
    for (int i = 0; i < (dimH + dimV); i++) {
      //Read all analog pins
      delayMicroseconds(100);
      ambThreshold[i] = analogRead(prAnalogIn[i]);
    }
  }
  
  //Output raw ADC values for all analog pins
  for (int i = 0; i < (dimH + dimV); i++) {
    //Read all analog pins
    analogOutputs[i] = analogRead(prAnalogIn[i]);
    delayMicroseconds(100);
  }

  //Write voltage string for strToList
  Serial.print("PINREAD");
  Serial.println();  
  String strOut = "[";
  for (int j = 0; j < (dimH+dimV); j++) {
    strOut = strOut + String(analogOutputs[j]);
    if (j != (dimH+dimV-1)) {
      strOut += ",";
    }
  }
  strOut += "]";
  Serial.print(strOut);
  Serial.println();

  Serial.print("BLOCKOUT");
  Serial.println();
  String blockOut = "[";
  for (int k = 0; k < dimH+dimV; k++) {
    if (analogOutputs[k] >= (ambThreshold[k]+laserDiff[k])) {
      blockOut = blockOut + String(k) + ",";
    }
  }
  if(blockOut.length() > 1) {
    blockOut.remove(blockOut.length()-1);
  }
  blockOut += "]";
  Serial.print(blockOut);
  Serial.println();
  delay(500);
}
