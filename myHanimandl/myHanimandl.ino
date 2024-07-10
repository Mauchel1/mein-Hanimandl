#include <HX711.h>       
#include <Button.h>
#include <Servo.h>
#include <Encoder.h>
#include <TFT.h>
#include <SPI.h>
#include "NoDelay.h"
#include <EEPROM.h>

#define MAXIMALGEWICHT 1000  // Maximales Gewicht

//const int TFT_MOSI 11  // Data out INTERN HARDWARE SPI pins
//const int TFT_SCLK 13  // Clock out INTERN HARDWARE SPI pins
const int TFT_CS =  10;
const int TFT_RST = 8; // Or set to -1 and connect to Arduino RESET pin
const int TFT_DC =  9;

const int LOADCELL_DOUT_PIN = 18;
const int LOADCELL_SCK_PIN = 19;

const int ENCODER_PIN_A = 3;
const int ENCODER_PIN_B = 2;
const int ENCODER_PIN_SW = 4;

const int SERVO_PIN = 5;
const int BUZZER_PIN = 6;

#define BUTTON_START_PIN A0 //14
#define BUTTON_STOP_PIN A1 //15
#define SWITCH_POS1_PIN A2 //16
#define SWITCH_POS2_PIN A3 //17

Button button_start(BUTTON_START_PIN); 
Button button_stop(BUTTON_STOP_PIN); 
Button button_select(ENCODER_PIN_SW); 
Button switch_pos1(SWITCH_POS1_PIN);
Button switch_pos2(SWITCH_POS2_PIN);

Encoder encoder(ENCODER_PIN_A, ENCODER_PIN_B);

Servo servo;

HX711 scale;

enum Modus {
   Modus_Setup,
   Modus_Manuel,
   Modus_Automatic
};

Modus currentModus;

int pos = 0;    // variable to store the servo position
int oldPos = 0;    // variable to store the old servo position
long oldPosition  = -999;
long oldReading = 0;

noDelay buzzertimer(1000, false);
bool buzzerOn = false;

TFT myScreen = TFT(TFT_CS, TFT_DC, TFT_RST);

char printout[4];

int counter = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);

  button_start.begin();
  button_stop.begin();
  button_select.begin();
  switch_pos1.begin();
  switch_pos2.begin();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  servo.attach(SERVO_PIN);
  pos = servo.read();

  myScreen.begin();

  myScreen.background(0,0,0);  // clear the screen with black

  myScreen.stroke(255,0,255);
  myScreen.text("Welcome",2,0);

  // increase font size for text in loop()
  myScreen.setTextSize(4);


  //buzzertimer.start();

  // set correct Modus
  
  if (switch_pos1.pressed())
  {
    currentModus = Modus_Manuel();
  }
  else if (switch_pos2.pressed()) 
  {
    currentModus = Modus_Automatic();
  }
  else
  {
    currentModus = Modus_Setup();
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  long newPosition = encoder.read();
  if (newPosition != oldPosition) {
    oldPosition = newPosition;
    Serial.print("pos: ");
    Serial.println(newPosition);
    pos = newPosition;
  }

  if(buzzertimer.update())
  {//Checks to see if set time has past
    if (!buzzerOn){
      tone(BUZZER_PIN, 1000); // Sende 1KHz Tonsignal
      buzzerOn = true;
    }
    else {
      noTone(BUZZER_PIN);     // Ton stoppen
      buzzerOn = false;
    }
  }

  if (button_start.pressed())
  {
    Serial.println("Button 1 pressed");
  }
  if (button_stop.pressed())
  {
    Serial.println("Button 2 pressed");
  }
  if (button_select.pressed())
  {
    Serial.println("Button 3 pressed");
  }
  if (switch_pos1.pressed())
  {
    Serial.println("Switch 1 changed");
  }
  if (switch_pos2.has_changed())
  {
    Serial.println("Switch 2 changed");
  }

  /*if (scale.is_ready()) {
    long reading = scale.read();
    if ((abs(reading - oldReading)) > 1500)
    {
      Serial.print("HX711 reading: ");
      Serial.println(reading);
      oldReading = reading;
    }
  } else {
    Serial.println("HX711 not found.");
  }*/

  if(oldPos != pos) {
    servo.write(pos);
    oldPos = pos;
    Serial.println(servo.read());
  }

  delay (50);

}
