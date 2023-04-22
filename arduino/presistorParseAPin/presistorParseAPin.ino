// Declare global variables
const int dimH = 2;
const int dimV = 2;
int prAnalogIn[]={A0,A1,A2,A3,A4,A5,A6};
double ambTHold = 0;
double analogOutputs[dimH + dimV];
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
  //Start serial output
  Serial.begin(9600);
  //Identify ambient light level on startup
  for (int i = 0; i < (dimH + dimV); i++) {
    //Read all analog pins
    delayMicroseconds(100);
    analogOutputs[i] = analogRead(prAnalogIn[i]);
  }
  ambTHold = avgAmbTHold(dimH+dimV, analogOutputs); // Use dimH+dimV instead of sizeof(analogOutputs)/sizeof(analogOutputs[0])
}

void loop() {
  // Check analog signal of all photoresistors
  for (int i = 0; i < (dimH + dimV); i++) {
    //Read all analog pins
    analogOutputs[i] = analogRead(prAnalogIn[i]);
    delayMicroseconds(100);
    Serial.print(String(i)+",");
    Serial.print(analogOutputs[i]);
    Serial.println();
  }
  Serial.print("Pin Read Cycle Complete");
  Serial.println();
}

//TODO: Wire button to allow for manual ambTHold reset
