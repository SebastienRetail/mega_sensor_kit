/* components
  Arduino Mega with sensor shield
  9v conector

  DIGITAL
  capacitive touch sensor
  flame sensor
  Passive Buzzer
  Piranha led module
  POT
  push button
  Temperature Sensor 18B20
  Ultrasonic HC-SR04

  ANALOG
  Ambient Light
  gas
  photocell
  rotary
  sound
  temperature

*/

/*pseudo code , what it does
the main routine in two parts
first it allows for various human interactions,
then takes a round of measurements and returns or acts in consequence :
if a sensor detects a problem it will ring mayday
else move on to next in loop
*/

/////////////////////////////////////////////////DECLARE EVERYTHING///
////////////////////////////////////////////////////////////////////
///////////////////////MUSIC////////////////////////////
////////////////////////////////////////////////////////////////////
/*
  https://gist.github.com/nicksort/4736535
*/

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

// const int ledPin1 = 38;
// const int ledPin2 = 39;



int counter = 2;

////////////////////////////////////////////////////////////////////
/////////////////DECLARE DIGITAL PINS/////////////////////
////////////////////////////////////////////////////////////////////
int PinPiranha = 4;

int PinPassiveBuzzer = 5; //set digital IO pin of the buzzer

int PinTouch = 6; // Connect Touch sensor on Digital Pin 6
int sensorMintouch = LOW;
int sensorMaxtouch = HIGH;

//DHT Sensor
// pin 8 needs to import a library
// http://www.pjrc.com/teensy/arduino_libraries/OneWire.zip
#include <OneWire.h>
int DS18S20_Pin = 7;
OneWire ds(DS18S20_Pin);

const int TempDigital = 8;
const int PinFlame = 9;
int StateFlame = 0; // variable for reading status used for the flame sensor
int PinPushButton = 10;
int PinYellowPushButton = 10;
int PinRedPushButton = 100; // to be defined
byte PinPIR = 11;
int StatePIR = 0;  // variable for reading status used for the PIR sensor

//ultrasonic
#define  UltraEchoPin  49 // Echo Pin
#define  UltraTrigPin  48 // Trigger Pin
//#define  LEDPin  4        // piranha LED from above

///////////////////////////////////////////////activity leds//
//leds for digital sensors
const int ledPin1 = 38;
const int ledPin2 = 43;
int PinLedPiranha = 22;
int PinLedPassiveBuzzer = 23; //set digital IO pin of the buzzer
int PinLedTouch = 24; // Connect Touch sensor on Digital Pin 6
int PinLedDS18S20 = 25;//DHT Sensor
int PinLedFlame = 26;
int PinLedPushButton = 27; //WILL BE RED PUSHBUTTON
int PinLedRedPushButton = 27; //WILL BE RED PUSHBUTTON
int PinLedPIR = 28;  // variable for reading status used for the PIR sensor
int PinLedUltra =  29;// Echo Pin

//leds for analog sensors
int PinLedPhotocell = 30;
int PinLedTemt6000 = 31; // ambient light
int PinLedMic = 32;
int PinLedGas = 33; 
int PinLedAlcohol = 34;
int PinLedAnalogRotaryPOT = 35;
int PinLedUnused1 = 36; // the other tmeperature sensor
int PinLedRed = 37; 
int PinLedGreen = 38;
int PinLedRotary2 = 39; // is the other rotary 
int PinLedRed2 = 40; 
int PinLedYellow = 41; // NOT STRONG


////////////////////////////////////////////////////////////////////
/////////////////DECLARE ANALOG PINS/////////////////////
////////////////////////////////////////////////////////////////////

int PinPhotocell = 0;
int PinTemt6000 = 1; // ambient light
int PinMic = 2;
int PinGas = 3;
int PinAlcohol = 4;
int AlcoholMin = 0;
int AlcoholMax = 5000;
int AnalogRotaryPOT = 5;
//int ANALOGpot2_1 = 38;
//int ANALOGpot2_2 = 39;
//int ANALOGpot2_3 = 40;

////////////////////////////////////////////////////////////////////
/////////////////DECLARE USE OF PINS/////////////////////
////////////////////////////////////////////////////////////////////

void setup() {
  pinMode(PinPiranha, OUTPUT);
  pinMode(PinPassiveBuzzer, OUTPUT);
  pinMode(PinTouch, INPUT);
  pinMode(PinPushButton, INPUT);
  pinMode(PinYellowPushButton, INPUT);
  pinMode(PinRedPushButton, INPUT);
  Serial.begin(9600);//temperature sensor

  pinMode(PinMic, INPUT);
  pinMode(PinGas, INPUT);
  pinMode(PinAlcohol, INPUT);
  pinMode(PinFlame, INPUT);//flame sensor
  pinMode(PinPIR, INPUT);
  pinMode(UltraTrigPin, OUTPUT);
  pinMode(UltraEchoPin, INPUT);
  pinMode(AnalogRotaryPOT, INPUT);

  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
//activity leds
//leds for digital sensors
pinMode(PinLedPiranha, OUTPUT);
pinMode(PinLedPassiveBuzzer, OUTPUT); 
pinMode(PinLedTouch, OUTPUT); 
pinMode(PinLedDS18S20, OUTPUT);//DHT
pinMode(PinLedFlame, OUTPUT);
pinMode(PinLedPushButton, OUTPUT);
pinMode(PinLedRedPushButton, OUTPUT);
pinMode(PinLedPIR, OUTPUT);
pinMode(PinLedUltra, OUTPUT);

pinMode(PinLedUnused1, OUTPUT);



pinMode(PinLedGreen, OUTPUT);
pinMode(PinLedRed, OUTPUT);
pinMode(PinLedRed2, OUTPUT);
pinMode(PinLedYellow, OUTPUT);

//leds for analog sensors
pinMode(PinLedPhotocell, OUTPUT);
pinMode(PinLedTemt6000, OUTPUT);// ambient light
pinMode(PinLedMic, OUTPUT);
pinMode(PinLedGas, OUTPUT);
pinMode(PinLedAlcohol, OUTPUT);
pinMode(PinLedAnalogRotaryPOT, OUTPUT);
pinMode(PinLedRotary2, OUTPUT);
}

////////////////MAIN ROUTINE//////////////////////////////

void loop() {
  
  Serial.println("Start Loop ! ");
  Piranha4();//blink led to show new round of sensors
  
  
  ////////////////INTERACT//////////////////////////////
  Serial.println("Wait 3 s to press pushbutton and play imperial march! ");
  delay(3000);
  YellowPushButton();//if pressed , it will play imperial march

  Piranha2();//blink led to show new round of sensors

  Serial.println("Wait 3 s to press touch button and test alcohol! ");
  delay(3000);
  Touch();//if pressed it will measure alcohol but maybe leave for something else ? 
  //RedPushButton();//if pressed it will measure alcohol

  Piranha2();//blink led to show new round of sensors
  Serial.println("Wait 1 s ! ");
  delay(1000);

  ////////////////MEASUREMENTS//////////////////////////////

  AmbientLight();
  //AnalogRotary();  //why constant values ?
  Flame();
  Gas();
  PhotoCell();
  PIR();
  SoundSensor();  //why constant values ?
  Termometer(); //broken ?  Ultrasonic();

  Serial.println("Wait 30 s for next round ! ");
  delay(30000);
}
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////END OF MAIN//////////
////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////
///////////////////////////////////////////AVAILABLE FUNCTIONS//////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Piranha1() { 
//blink led twice
  Serial.println("Run Piranha 2 ! ");
      digitalWrite(PinLedPiranha, HIGH);
    delay(400);
  unsigned char i;
  for (i = 0; i < 1; i++);
  {
    Serial.println(i);
    digitalWrite(PinPiranha, HIGH); //Turn off led
    delay(500);
    digitalWrite(PinPiranha, LOW); //Turn on led
    delay(50);
  }    
  digitalWrite(PinLedPiranha, LOW);
}

void Piranha2() { 
//blink led twice
  Serial.println("Run Piranha 2 ! ");
    digitalWrite(PinLedPiranha, HIGH);
    delay(40);
  unsigned char i;
  for (i = 0; i < 2; i++);
  {
    Serial.println(i);
    digitalWrite(PinPiranha, HIGH); //Turn off led
    delay(200);
    digitalWrite(PinPiranha, LOW); //Turn on led
    delay(50);
  }
  digitalWrite(PinLedPiranha, LOW);
}

void Piranha4() { 
//blink led to show new round of sensor measurements
  Serial.println("Run Piranha 4 ! ");
  digitalWrite(PinLedPiranha, HIGH);
    delay(400);
  unsigned char i;
  for (i = 0; i < 4; i++);
  {
    Serial.println(i);
    digitalWrite(PinPiranha, HIGH); //Turn off led
    delay(700);
    digitalWrite(PinPiranha, LOW); //Turn on led
    delay(50);
  }
  digitalWrite(PinLedPiranha, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void Buzzer() {
  Serial.println("Run Buzzer ! ");
  	digitalWrite(PinLedPassiveBuzzer, HIGH);
    delay(400);
  unsigned char i;//define variable
  for (i = 0; i < 3; i++); // output a frequency sound
  {
    Serial.println(i);
    digitalWrite(PinPassiveBuzzer, HIGH); // sound
    delay(1000);
    digitalWrite(PinPassiveBuzzer, LOW); //not sound
    delay(50);
  }
digitalWrite(PinLedPassiveBuzzer, LOW);
}

void BuzzerOK() {

  Serial.println("Run BuzzerOK ! ");
  	digitalWrite(PinLedPassiveBuzzer, HIGH);
  delay(400);
  unsigned char i;//define variable
  // while(1)
  // { for(i=0;i<3;i++)// output a frequency sound
  // { digitalWrite(PinPassiveBuzzer,HIGH);// sound
  // delay(1);//delay1ms
  // digitalWrite(PinPassiveBuzzer,LOW);//not sound
  // delay(1);//ms delay
  // }
  for (i = 0; i < 2; i++); // output a frequency sound
  {
    Serial.println(i);
    digitalWrite(PinPassiveBuzzer, HIGH); // sound
    digitalWrite(PinLedGreen, HIGH); // sound
    delay(200);//2ms delay
    digitalWrite(PinPassiveBuzzer, LOW); //not sound
    digitalWrite(PinLedGreen, LOW); // sound
    delay(100);//2ms delay
  }
    digitalWrite(PinLedPassiveBuzzer, LOW);
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
void Mayday() {
  Serial.println("Run Mayday ! ");

  while (digitalRead(PinTouch) == LOW) { //Read Touch sensor signal
    Serial.println("Mayday ! ");
    MaydaySound();
    delay(10);
  }
}

void Mayday2() {
  Serial.println("Run Mayday ! ");
  // read the sensor:
  int sensorReading = digitalRead(PinTouch);

  // map the sensor range to a range of four options:
  int range = map(sensorReading, sensorMintouch, sensorMaxtouch, 0, 1);

  // do something different depending on the range value:
  switch (range) {
    case 0:    // no one is touching the sensor
      Serial.println("no touch");
      MaydaySound();
      break;
    case 1:    // your finger touches the sensor
      Serial.println("touch prooves risk is aknowledged and turns off mayday");
      break;
  }
  delay(1);        // delay in between reads for stability
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Touch() {
  Serial.println("Run Touch ! ");
  	    digitalWrite(PinLedTouch, HIGH);
    delay(400);
  if (digitalRead(PinTouch) == HIGH) { //Read Touch sensor signal
    firstVariant();
    Buzzer();
    Alcohol();
    BuzzerOK();
  }
  else {
    Serial.println("not Touched ! ");
  }
    digitalWrite(PinLedTouch, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Termometer() {
  Serial.println("Run Termometer ! ");
  	digitalWrite(PinLedDS18S20, HIGH);
    delay(400);
  Serial.begin(9600);
  float temperature = getTemp();
  Serial.println("temperature : ");
  Serial.println(temperature);
  //delay(100); //just here to slow down the output so it is easier to read
  if (temperature > 30) {
    Serial.println("!!!!!! temperature is over 30 Celsius !!!!!!");
    Piranha4();
    secondVariant();
    Mayday();
  }    
  digitalWrite(PinLedDS18S20, LOW);
}

float getTemp() {
  Serial.println("Run gettemp ! ");
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  Serial.begin(9600);
  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return -1000;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }
  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.println("Device is not recognized");
    return -1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Flame() {
  Serial.println("Run Flame ! ");
	digitalWrite(PinLedFlame, HIGH);
    delay(400);
  Serial.begin(9600);
  // read the state of the value:
  StateFlame = digitalRead(PinFlame);
  Serial.println("FlameState : ");
  Serial.println(StateFlame);
  if (StateFlame == 0) {
    Serial.println("!!! FLAME HAS BEEN SENSED !!!");
    firstVariant();
    Mayday();
  }
    digitalWrite(PinLedFlame, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////


void RedPushButton() {
  Serial.println("Run red pushbutton ! ");
  digitalWrite(PinLedRedPushButton, HIGH);
      delay(400);
  delay(100);
  int val = digitalRead(PinRedPushButton);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    Piranha2();
    Imperial();
  }
  else {
    Serial.println("Red Button not Pushed ! ");
  }    
  digitalWrite(PinLedRedPushButton, LOW);
}

void YellowPushButton() {
  Serial.println("Run yellow pushbutton ! ");
  //digitalWrite(PinLedPushButton, HIGH);
  digitalWrite(PinLedYellow, HIGH);
    delay(400);
  delay(100);
  int val = digitalRead(PinYellowPushButton);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    Piranha2();
    Imperial();
  }
  else {
    Serial.println("Yellow Button not Pushed ! ");
  }    
  //digitalWrite(PinLedPushButton, LOW);
  digitalWrite(PinLedYellow, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void PIR() {
 Serial.println("Run PIR ! ");
      digitalWrite(PinLedPIR, HIGH);
    delay(400);

  byte StatePIR = digitalRead(PinPIR);
  digitalWrite(PinPiranha, StatePIR);
  if (StatePIR == 1) {
    Serial.println("Somebody is in this area!");
    Piranha1();
    Buzzer();
  }
  else {
    Serial.println("No one!");
  }

  delay(200);    
  digitalWrite(PinLedPIR, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void AmbientLight() {
  Serial.println("Run AmbientLight ! ");
      digitalWrite(PinLedTemt6000, HIGH);
    delay(40);
  int value = analogRead(PinTemt6000);
  Serial.println("AmbientLight : ");
  Serial.println(value);
  delay(100); //only here to slow down the output so it is easier to read 
  digitalWrite(PinLedTemt6000, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void PhotoCell() {
  Serial.println("Run PhotoCell ! ");
      digitalWrite(PinLedPhotocell, HIGH);
    delay(400);
  int valPhotocell = 0;
  Serial.begin(9600);
  valPhotocell = analogRead(PinPhotocell);
  Serial.println("PhotoCell : ");
  Serial.println(valPhotocell, DEC);
  delay(50);
  if (valPhotocell > 900 ) {
    Mayday();
  }    
  digitalWrite(PinLedPhotocell, LOW);
  //  else digitalWrite(PinPiranha,LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void SoundSensor() {
  Serial.println("Run SoundSensor ! ");
      digitalWrite(PinLedMic, HIGH);
    delay(400);

  int valSound;
  valSound = analogRead(PinMic);
  Serial.println("Sound : ");
  Serial.println(valSound, DEC);
  delay(100);
  if (valSound > 300 ) {
    Serial.println("sound above quiet : There's somebody in the room");
    Piranha1();
    //Mayday();
  }
  //else digitalWrite(PinPiranha,LOW);    
  digitalWrite(PinLedMic, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Gas() {
  Serial.println("Run Gas ! ");
      digitalWrite(PinLedGas, HIGH);
    delay(400);
    
  Serial.begin(9600);
  int valGas;
  valGas = analogRead(PinGas); //Read Gas value from analog 3
  Serial.println("Gas : ");
  Serial.println(valGas, DEC); //Print the value to serial port
  delay(100);

  if (valGas > 500) {
    digitalWrite(PinPiranha, HIGH);
    Mayday();
    //Imperial();
  }
  //else digitalWrite(PinPiranha,LOW);
  digitalWrite(PinLedGas, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Alcohol() {
  delay(2000); // leave more time for the drunk ! 
  Serial.println("Run Alcohol ! ");
      digitalWrite(PinLedAlcohol, HIGH);
    delay(400);

  Serial.begin(9600);
  int valAlcohol;
  valAlcohol = analogRead(PinAlcohol); //Read Gas value from analog 4
  Serial.println("Alcohol : ");
  Serial.println(valAlcohol, DEC); //Print the value to serial port
  delay(100);
  int rangeAlcohol = map(valAlcohol, AlcoholMin, AlcoholMax, 0, 4);

  // do something different depending on the range value:
  switch (rangeAlcohol) {
    case 0:    // your hand is on the sensor
      Serial.println("keep on drinking water");
      //Play first Variant
      BuzzerOK();
      break;
    case 1:    // your hand is close to the sensor
      Serial.println("had a drink");
      // Play second Variant
      Buzzer();
      break;
    case 2:    // your hand is a few inches from the sensor
      Serial.println("had a few drinks");
      //Play first section
      firstVariant();
      break;
    case 3:    // your hand is nowhere near the sensor
      Serial.println("do you want more ?");
      //Play second section
      secondVariant();
      break;
    case 4:    // your hand is nowhere near the sensor
      Serial.println("you must be drunk");
      Imperial();
      Mayday();
      break;
  }
  delay(1);        // delay in between reads for stability    
  digitalWrite(PinLedAlcohol, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Ultrasonic() {
  Serial.println("Run Ultrasonic ! ");
      digitalWrite(PinLedUltra, HIGH);
    delay(400);

  /* The following UltraTrigPin/UltraEchoPin cycle is used to determine the
    distance of the nearest object by bouncing soundwaves off of it. */
  int maximumRange = 200; // Maximum range needed
  int minimumRange = 0; // Minimum range needed
  long duration, distance; // Duration used to calculate distance
  Serial.begin(9600);

  digitalWrite(UltraTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(UltraTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(UltraTrigPin, LOW);
  duration = pulseIn(UltraEchoPin, HIGH);
  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration / 58.2;
  if (distance >= maximumRange || distance <= minimumRange) {
    /* Send a negative number to computer and Turn LED ON
      to indicate "out of range" */
    Serial.println(" -1 : ultrasonic is out of range");
    digitalWrite(PinPiranha, HIGH);
  }
  else {
    /* Send the distance to the computer using Serial protocol, and
      turn LED OFF to indicate successful reading. */
    Serial.println(" distance : ");
    Serial.println(distance);
	digitalWrite(PinLedUltra, LOW);
  }
  //Delay 50ms before next reading.
  //delay(50);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void AnalogRotary() {
  Serial.println("Run Rotary ! ");
      digitalWrite(PinLedAnalogRotaryPOT, HIGH);
    delay(400);

  int val;
  val = analogRead(AnalogRotaryPOT); //Read rotation sensor value from analog 0
  Serial.println("analog rotation : ");
  Serial.println(val, DEC); //Print the value to serial port
  delay(100);
  //play sound depending on rotation ?
    digitalWrite(PinLedAnalogRotaryPOT, LOW);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

void Imperial(){
  Serial.println("Run Imperial ! ");
  firstSection();
  secondSection();
  firstVariant();
  secondSection();
  secondVariant();
}

void beep(int note, int duration) {
  //Play tone on buzzer
  tone(PinPassiveBuzzer, note, duration);

  //Play different LED depending on value of 'counter'
  if (counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  } else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }

  //Stop tone on buzzer
  noTone(PinPassiveBuzzer);

  delay(50);

  //Increment counter
  counter++;
}

void firstSection() {
  Serial.println("Run firstSection ! ");
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);
}

void secondSection() {
  Serial.println("Run secondSection ! ");
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);
  beep(fSH, 250);

  delay(325);

  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);
  beep(cSH, 175);
  beep(cH, 125);
  beep(b, 125);
  beep(cH, 250);

  delay(350);
}

void firstVariant() {
  Serial.println("Run firstVariant ! ");
  beep(f, 250);
  beep(gS, 500);
  beep(f, 350);
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);
  beep(cH, 125);
  beep(eH, 650);

  delay(500);
}

void secondVariant() {
  Serial.println("Run secondVariant ! ");
  beep(f, 250);
  beep(gS, 500);
  beep(f, 375);
  beep(cH, 125);
  beep(a, 500);
  beep(f, 375);
  beep(cH, 125);
  beep(a, 650);

  delay(650);
}

void MaydaySound() {
  int i;
  for (i = 0; i < 4; i++); //
  {
    Serial.println("Run maydaysound ! ");
    beepMayday(a, 250);
    beepMayday(a, 250);
    beepMayday(a, 250);
    delay(150);
    beepMayday(a, 500);
    beepMayday(a, 500);
    beepMayday(a, 500);
    delay(150);
    beepMayday(a, 250);
    beepMayday(a, 250);
    beepMayday(a, 250);
    delay(650);
  }
  Serial.println("delay 3000 ! leave time to press touch button");
  delay (3000);
}


void beepMayday(int note, int duration) {
  //Play tone on buzzer
  tone(PinPassiveBuzzer, note, duration);

  //Play different LED depending on value of 'counter'
  if (counter % 2 == 0)
  {
    digitalWrite(PinLedRed, HIGH);
    delay(duration);
    digitalWrite(PinLedRed, LOW);
  } else
  {
    digitalWrite(PinLedRed2, HIGH);
    delay(duration);
    digitalWrite(PinLedRed2, LOW);
  }

  //Stop tone on buzzer
  noTone(PinPassiveBuzzer);

  delay(50);

  //Increment counter
  counter++;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////END OF PAGE//////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
