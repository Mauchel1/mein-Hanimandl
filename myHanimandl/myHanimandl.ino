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

enum SetupMenu {
  Setup_Calibrate, //first
  Setup_Servo,
  Setup_Glas,
  Setup_Automatic,
  Setup_Parameter  //last
};

static const char * const SetupMenuEntries[] = {
  [Setup_Calibrate] = "Kalibrieren",
  [Setup_Servo] = "Servo",
  [Setup_Glas] = "Glas",
  [Setup_Automatic] = "Automatik",
  [Setup_Parameter] = "Parameter"
};

Modus currentModus;
Modus lastModus = -1;

int taraweight = 500; //weight to calibrate the scale in g

int angle = 0;    // variable to store the servo position
int minAngle = 0;    // variable to store the minimum servo position
int maxAngle = 180;    // variable to store the maximum servo position
int oldAngle = 0;    // variable to store the old servo position
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

  servo.write(minAngle);
  servo.attach(SERVO_PIN);//,1000,2000);
  angle = minAngle;

  myScreen.begin();

  //buzzertimer.start();

  // set correct Modus
  
  if ( switch_pos1.PRESSED)
  {
    currentModus = Modus_Manuel;
  }
  else if (switch_pos2.PRESSED) 
  {
    currentModus = Modus_Automatic;
  }
  else
  {
    currentModus = Modus_Setup;
  }

  ReadEncoder();
}

void handleButton()
{

}

long ReadScale(){
  if (scale.is_ready()) {
    long reading = round(scale.get_units(2));
    /*if ((abs(reading - oldReading)) > 1500)
    {
      oldReading = reading;
    }*/
    return reading;
  } else {
    return -999;
  }
}

long ReadEncoder() //gives the change of the encoder
{
  long newPosition = encoder.read()/4;
  if (newPosition != oldPosition) 
  {
    long returnvalue = newPosition-oldPosition;
    oldPosition = newPosition;    
    return returnvalue;
  }
  return 0;
}

void ChangeAngle(int change)
{
  if ((angle + change) > maxAngle){return;}
  if ((angle + change) < minAngle){return;}
  angle += change;
}


void loop() {

  StateMachine();

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

  if (switch_pos1.released() && switch_pos2.RELEASED || switch_pos2.released() && switch_pos1.RELEASED)
  {
    currentModus = Modus_Setup;
    Serial.println("Modus_Setup");
  }  
  else if (switch_pos1.pressed())
  {
    Serial.println("Modus_Manuel");
    currentModus = Modus_Manuel;
  }
  else if (switch_pos2.pressed())
  {
    currentModus = Modus_Automatic;
    Serial.println("Modus_Automatic");
  }

  if(oldAngle != angle) {
    servo.write(angle);
    oldAngle = angle;
    Serial.print("Servowrite: ");
    Serial.println(angle);
  }

  delay (50);

}
