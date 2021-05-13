// Uduino Default Board
#include<Uduino.h>
Uduino uduino("uduinoBoard"); // Declare and name your object

// variables for two pixel rings
#include <Adafruit_NeoPixel.h> // import rgb circle library thing
int ring1Pin = 10; // pin number for ring 
int ring2Pin = 11;
#define NUMPIXELS 20 // define max number of leds
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, ring1Pin, NEO_GRB + NEO_KHZ800); // create our circular light as a object
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, ring2Pin, NEO_GRB + NEO_KHZ800); // create our circular light as a object
int pixelI = 10; // intensity of color which is changes by unity later 


// variables 
int index1 = 10;
int index2 = 20;

int lowCut = 12;
int highCut = 22;
int oldLowCut;
int oldHighCut;

// variables for first encoder
static int pinA = 2; // Our first hardware interrupt pin is digital pin 2
static int pinB = 3; // Our second hardware interrupt pin is digital pin 3
volatile byte aFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte bFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// variables for first encoder
static int pinC = 4; // Our first hardware interrupt pin is digital pin 2
static int pinD = 5; // Our second hardware interrupt pin is digital pin 3
volatile byte cFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte dFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading2 = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// variables for first encoder
static int pinE = 6; // Our first hardware interrupt pin is digital pin 2
static int pinF = 7; // Our second hardware interrupt pin is digital pin 3
volatile byte eFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte fFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading3 = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

// variables for first encoder
static int pinG = 8; // Our first hardware interrupt pin is digital pin 2
static int pinH = 9; // Our second hardware interrupt pin is digital pin 3
volatile byte gFlag = 0; // let's us know when we're expecting a rising edge on pinA to signal that the encoder has arrived at a detent
volatile byte hFlag = 0; // let's us know when we're expecting a rising edge on pinB to signal that the encoder has arrived at a detent (opposite direction to when aFlag is set)
volatile byte reading4 = 0; //somewhere to store the direct values we read from our interrupt pins before checking to see if we have moved a whole detent

//variables for buttons
int button1 = 0;
int button2 = 1;

int buttonState1 = 0;
int buttonState2 = 0;

int oldButtonState1 = 0;
int oldButtonState2 = 0;

// variables for LEDS
int firstPin = 12;
int lastPin = 22;


void setup()
{

  
  Serial.begin(9600);
  // first encoder
  pinMode(pinA, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinB, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(pinA, PinA,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(pinB, PinB,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  // second enconder
  pinMode(pinC, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinD, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(pinC, PinC,RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(pinD, PinD,RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
   // third encoder
  pinMode(pinE, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinF, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(pinE, PinE, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(pinF, PinF, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)
  // fourth enconder
  pinMode(pinG, INPUT_PULLUP); // set pinA as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  pinMode(pinH, INPUT_PULLUP); // set pinB as an input, pulled HIGH to the logic voltage (5V or 3.3V for most cases)
  attachInterrupt(pinG, PinG, RISING); // set an interrupt on PinA, looking for a rising edge signal and executing the "PinA" Interrupt Service Routine (below)
  attachInterrupt(pinH, PinH, RISING); // set an interrupt on PinB, looking for a rising edge signal and executing the "PinB" Interrupt Service Routine (below)

  //pinMode(8, OUTPUT);
  
  
  // buttons
  pinMode(button1, INPUT_PULLUP); // taking the input from buttons
  pinMode(button2, INPUT_PULLUP); 
  // LED rings 
  pixels.begin();
  pixels2.begin();

  // Initializng LEDs
  for(int i = firstPin; i <= lastPin; i++){
    pinMode(i, OUTPUT);
  }


  // commands added to uduino which can be called from Unity. The commands triggers the corresponding function in this arduino code. 
  uduino.addCommand("turnOn1", turnOnPixel1); 
  uduino.addCommand("turnOn2", turnOnPixel2);
  uduino.addCommand("changeIntensity", changeIntensity);
  uduino.addCommand("BandPass", BandPass);
  
}

void loop()
{

  // updating in order to get input from Unity
  uduino.update();

  // reading the state of the buttons
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  
  // checking if the state of the button has change
  if(buttonState1 != oldButtonState1){
    if (buttonState1 == HIGH){ // if state is high send code - 3
      Serial.println(1);
    }
    if (buttonState1 == LOW){ // if low send 4
      Serial.println(2);
    }
    oldButtonState1 = buttonState1;
  }


  if(buttonState2 != oldButtonState2){ // same as with the other button
    if (buttonState2 == HIGH){
      Serial.println(3);
    }
    if (buttonState2 == LOW){
      Serial.println(4);
    }
    oldButtonState2 = buttonState2;
  }

   
  // for loop lighting up each LED on both rings, depending on the value index1 and index2. 
  for(int i=0;i<NUMPIXELS;i++)
      {
        if (i < index1){ // if the LED is under the index(encoder) value, then the LEDS will light
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels.setPixelColor(i, pixels.Color(0,pixelI,0)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
        else{ // else it will not light
        pixels.setPixelColor(i, 0); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
        if (i < index2){ // if the LED is under the index(encoder) value, then the LEDS will light
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        pixels2.setPixelColor(i, pixels2.Color(0,pixelI,0)); // Moderately bright green color.
        pixels2.show(); // This sends the updated pixel color to the hardware.
        }
        else{ // else it will not light
        pixels2.setPixelColor(i, 0); // Moderately bright green color.
        pixels2.show(); // This sends the updated pixel color to the hardware.
        }
    }

   // for loop lighting the LED strip, depending on the highcut and lowcut from unity (mapped from 12-22
   if(highCut != oldHighCut || lowCut != oldLowCut){
   for(int i=12; i<=22; i++){
      if(i >= lowCut && i <= highCut){
        digitalWrite(i, HIGH);
      }
      else{
        digitalWrite(i, LOW);
      }
   }
   oldHighCut = highCut;
   oldLowCut = lowCut;
   }

   
}

// functions that takes an argument from unity when called. 
void turnOnPixel1(){
  index2 = atoi(uduino.getParameter(0)); // index1 (int controlling number of pixels which should light up) becomes the first argument sent from unity when function is called
}

// functions that takes an argument from unity when called.
void turnOnPixel2(){
  index1 = atoi(uduino.getParameter(0));// index1 (int controlling number of pixels which should light up) becomes the first argument sent from unity when function is called
} 

// functions that takes an argument from unity when called.
void changeIntensity(){
  pixelI = atoi(uduino.getParameter(0));
}

void BandPass(){
  lowCut = atoi(uduino.getParameter(0)); 
  highCut = atoi(uduino.getParameter(1)); 
}



// interrupt functions for all encoders: 
void PinA(){
  cli(); //stop interrupts happening before we read pin values
  // Serial.println(PIND);
  //Serial.println(20);
  reading = PIND & 12; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading == 12 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(5);
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == 4) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinB(){
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 12; //read all eight pin values then strip away all but pinA and pinB's values
  //Serial.println(20);
  if (reading == 12 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(6);//increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == 4) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinC(){
  cli(); //stop interrupts happening before we read pin values
  reading2 = PIND & 60; // read all eight pin values then strip away all but pinC and pinD's values
  if(reading2 == 60 && cFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(7);
    dFlag = 0; //reset flags for the next turn
    cFlag = 0; //reset flags for the next turn
  }
  else if (reading2 == 44) dFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinD(){
  cli(); //stop interrupts happening before we read pin values
  reading2 = PIND & 60; //read all eight pin values then strip away all but pinC and pinD's values
  if (reading2 == 60 && dFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(8);//increment the encoder's position count
    dFlag = 0; //reset flags for the next turn
    cFlag = 0; //reset flags for the next turn
  }
  else if (reading2 == 44) cFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}


// interrupt functions for all encoders: 
void PinE(){
  cli(); //stop interrupts happening before we read pin values
  reading3 = PIND & 252;
  //Serial.println(reading3);
  //reading3 = PIND & 192; // read all eight pin values then strip away all but pinA and pinB's values
  if(reading3 == 252 && eFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(9);
    fFlag = 0; //reset flags for the next turn
    eFlag = 0; //reset flags for the next turn
  }
  else if (reading3 == 124) fFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinF(){
  cli(); //stop interrupts happening before we read pin values
  reading3 = PIND & 252;
  //reading3 = PIND & 192; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading3 == 252 && fFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(10);
    fFlag = 0; //reset flags for the next turn
    eFlag = 0; //reset flags for the next turn
  }
  else if (reading3 == 188) eFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinG(){
  cli(); //stop interrupts happening before we read pin values
  reading4 = PINB & 3; // read all eight pin values then strip away all but pinC and pinD's values
  //Serial.println(reading4);
  if(reading4 == 3) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(11);
    gFlag = 0; //reset flags for the next turn
    hFlag = 0; //reset flags for the next turn
  }
  else if (reading4 == 2) hFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void PinH(){
  cli(); //stop interrupts happening before we read pin values
  reading4 = PINB & 3; //read all eight pin values then strip away all but pinC and pinD's values
  //Serial.println(reading4);
  if (reading4 == 3 && hFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    Serial.println(12);//increment the encoder's position count
    gFlag = 0; //reset flags for the next turn
    hFlag = 0; //reset flags for the next turn
  }
  else if (reading2 == 2) gFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}
