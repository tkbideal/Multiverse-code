//Author: Taiwo A. Adegbuyi
//Title: Lab Finals: Takes sound input from 3 different Sensors and also display their values on the LCD
//Description: Read the distance sensor, touch sensor and sound sensor, and display their values on the LCD screen 
//Date: 02/20/2024
//Version: 2.3.2

#include <Wire.h>                     // Tells the compiler to include the I2C wire library
#include <LiquidCrystal_I2C.h>        // Tells the compiler to include the LCD library

LiquidCrystal_I2C lcd(0x27, 16, 2);   // I2C address may vary, replace with the correct address
const int PinButt = 4;                // Digital pin for the button
const int SndSnsPin = A0;             // Analog pin for the sound sensor
const int trgPin = 10;                // Digital pin for the ultrasonic sensor trigger
const int echPin = 11;                // Digital pin for the ultrasonic sensor echo
const int touchSnsPin = 7;            // Digital pin for the touch sensor

enum SensorType {                    // This is data type used to assign names to constants
  Snd_Sns,                           // This declares the sound sensor as a constant
  Distnc_Sns,                        // This declares the distance sensor as a constant
  Touch_Sns                          // This declares the touch sensor as a constant
};

SensorType curntSns = Snd_Sns;       // Declares the sound sensor as the current sensor
bool touchSnsState = false;          // Initializes the touch sensor as off

void setup() {
  Serial.begin(9600);                //Tells the Arduino circuit to communicate using port 9600
  lcd.begin(16, 2);                  //The LCD address for data input
  lcd.backlight();                   //This activates the lcd backlight 
  pinMode(PinButt, INPUT_PULLUP);    //This uses the circuit internal resistor for the push board
  pinMode(trgPin, OUTPUT);           //Sets the Distance Sensor pin for output mode
  pinMode(echPin, INPUT);            //Sets the Distance Sensor pin for input mode
  pinMode(touchSnsPin, INPUT);       //Sets the Touch Sensor pin for input mode
}

void loop() {
  updateSensor();                    //Updates the sensors after each loop

  lcd.clear();                       //Clears the LCD screen and ready for fresh data to display
  lcd.setCursor(0, 0);               //Sets the LCD screen cursor to the start, i.e column 1, row 1

  switch (curntSns) {                //Switches between the sensor types to display on the screen
    case Snd_Sns:                    //Sound switch data when the sound displays
      lcd.print("Sound: ");          //Displays "Sound: " on the LCD screen
      lcd.print(readSoundSensor());  //Then displays the current sound's value in he front of "Sound: "
      lcd.print(" dB");              //Also displays the dB in the front of "Sound: "
      break;
    case Distnc_Sns:                    //Switches to Distance sensor to display on the screen
      lcd.print("Distance: ");          //Displays "Distance: " on that LCD line
      lcd.print(readDistanceSensor());  //Then displays the current Distance value in the front of "Distance: "
      lcd.print(" cm");                 //Also displays the cm in the front of "Distance: "
      break;
    case Touch_Sns:                                           //Switches to Touch sensor to display on the screen
      lcd.print("Touch: ");                                   //Displays "Touch: " on the LCD screen
      lcd.print(touchSnsState ? "Pressed" : "Not Pressed");   //Displays the touch sensor state when it is pressed or not
      break;
  }

  delay(1000);                          //Delays for the LCD displays to be readable for human
}

int readSoundSensor() {                 //This is the sound sensor reading logic
  return random(40, 65);                //This is random value applicable for human hearing
}

int readDistanceSensor() {                            //This is distance sensor reading logic
  digitalWrite(trgPin, LOW);                         //Turns the distance trig pin off
  delayMicroseconds(2);                               //Delays it for 2 micro seconds
  digitalWrite(trgPin, HIGH);                        //Turns the distance trig pin on
  delayMicroseconds(10);                              //Delays it for 10 micro seconds
  digitalWrite(trgPin, LOW);                         //Then turns the distance trig pin off
  return pulseIn(echPin, HIGH) * 0.034 / 2;           //Returns the distance to use the echo value in the loop, using the calculted centimeter 
}

void updateSensor() {                                             //Creates a function to use to updates the button when press after each loop
  if (digitalRead(PinButt) == LOW) {                              //Sets a condition for button know when off 
   
    curntSns = static_cast<SensorType>((curntSns + 1) % 3);       //When button is pressed, switch to the next sensor type
    delay(1000);                                                  //Delays to prevent rapid switching
  }

  touchSnsState = digitalRead(touchSnsPin) == HIGH;               //Updates the touch sensor state
}
