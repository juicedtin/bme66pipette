// Declare global variables
int dimH = 3;
int dimV = 3;
int pinMXEN = 1;
int pinMXA0 = 2;
int pinMXA1 = 3;
int pinMXA2 = 4;
int pinAnalogIn = A0;
int mxTT[9] = {0000,1000,1100,1010,1110,1001,1101,1011,1111};
double ambTHold = 0;
double analogOutputs[dimH+dimV];

void setup() {
  // Set pin modes for multiplexer control pins
  pinMode(pinMXEN, OUTPUT);
  pinMode(pinMXA0, OUTPUT);
  pinMode(pinMXA1, OUTPUT);
  pinMode(pinMXA2, OUTPUT);
  
  // Set pin mode for analog input pin
  pinMode(pinAnalogIn, INPUT);
}

//Converts the target photoresistor into the high/low boolean of a specific output pin to multiplexer
bool mxGetPR(int tgtR, int tt[9], String pin, int size) {
  //Output pin high/low in the order EN, A0, A1, A2
  int pinStates[4] = {tt[tgtR]/1000, (tt[tgtR]/100) % 10, (tt[tgtR]/10) % 10, tt[tgtR] % 10};
  //Return booleans according to the pinStates array, return false as default
  // Use size argument instead of sizeof(analogOutputs)/sizeof(analogOutputs[0])
  // Rest of the function code...
}

void loop() {
  //Get averaged ambient light value for comparison
  for (int i = 0; i < (dimH+dimV); i++) {
    //Set pinModes according to the index of the photoresistor
    pinMode(pinMXEN, mxGetPR(i, mxTT, "EN", dimH+dimV)); // Pass size of analogOutputs array as argument
    pinMode(pinMXA0, mxGetPR(i, mxTT, "A0", dimH+dimV)); // Pass size of analogOutputs array as argument
    pinMode(pinMXA1, mxGetPR(i, mxTT, "A1", dimH+dimV)); // Pass size of analogOutputs array as argument
    pinMode(pinMXA2, mxGetPR(i, mxTT, "A2", dimH+dimV)); // Pass size of analogOutputs array as argument
    delayMicroseconds(100);
    analogOutputs[i] = analogRead(pinAnalogIn);
  }
  ambTHold = std::accumulate(analogOutputs, analogOutputs + (dimH+dimV), 0); // Use dimH+dimV instead of sizeof(analogOutputs)/sizeof(analogOutputs[0])
  // Check analog signal of all photoresistors
}
