//-------------------------------------------------------------------------------------------------//
// This Program is made for the Altspark Arcus Module for the 2024 Camosun Capstone Project,
// this code will go into the esp32-devkitm-1 that will control the AC-DC Voltage Covertor
// to charge a 400VDC Leaf Battery.
//
// To program the Esp32 you'll have to download a library and a board library.
// The Library you need to download: LiquidCrystal I2C By Marco Schwartz.
// You can find this in the Manage Library tab under Tools on the top of the Arduino IDE.
// The Board Library you need to download: esp32 By Espressif Systems.
// You can find this in the Board Manager tab under Tools on the top of the Arduino IDE.
//
// This program was programed on the Arduino Legacy IDE.
//
// By Kongphop Amphandang
// Date Oct/15/2024
//-------------------------------------------------------------------------------------------------//
// LCD Display Library
#include <LiquidCrystal_I2C.h>

// The Output Pin for the PWM: GPIO 2
const int ledPin = 2;

// Setting LCD Screen Settings
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Setting PWM properties
const int freq = 30000;       // PWM Frequency setting
const int resolution = 8;     // PWM Resolution setting
int dutyCycle = 0;            //0-255 = 0-100% DutyCycle
float Voltage = 0;
float Current = 0;

// DutyCycle text setup (Semi obsolete)
int dutyCycletext = 0;
int text = 0;
int Start = 0;
int textv = 0;
int Startv = 0;

// Screen Move (obsolete)
int Move = 1;
int Stop = 0;
int Clear = 0;
int Stope = 0;

float CVolt = 0;              // Voltage at current reader

//SetCurrent & Module status Value setting
float SetCurrent = 15;
int Batfull = 0;
int Baterror = 0;
int Batcharge = 0;
int hys = 0;

//Timers Settings
int Timer1 = 0;
int Timer2 = 0;
int Timer3 = 0;
int Timer4 = 0;
int Timer5 = 0;
int Timer6 = 0;
int TS1 = 0;
int TS2 = 0;
int TS3 = 0;
int TS4 = 0;
int TS5 = 0;
int TS6 = 0;

//-------------------------------------------setup------------------------------------------//
void setup() {

  // Pinmodes for LEDs
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(14, OUTPUT);



  // Configure PWM Settings
  ledcAttach(ledPin, freq, resolution);

  // Setting LCD Screen
  lcd.init();               // Initialize the LCD
  lcd.backlight();          // Turn on the backlight

  // Setting Team Name for startup
  lcd.setCursor(0, 0);
  lcd.print("--------------------");
  lcd.setCursor(3, 1);
  lcd.print("AltSpark Tech");
  lcd.setCursor(3, 2);
  lcd.print("ARCUS Module");
  lcd.setCursor(0, 3);
  lcd.print("--------------------");

  // LED Test On
  digitalWrite(26, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(27, HIGH);

  delay(2000);

  // LED Test Off
  digitalWrite(26, LOW);
  digitalWrite(14, LOW);
  digitalWrite(27, LOW);

  // LED Test sequence Program
  StartL();

  // Clear Screen
  lcd.clear();
}
//------------------------------------------------------------------------------------------//
//-----------------------------------------LOOP-------------------------------------------------//
void loop() {

  // Setting Main Screen info
  lcd.setCursor(0, 0);
  lcd.print("Voltage");
  lcd.setCursor(0, 2);
  lcd.print("Current");
  lcd.setCursor(10, 0);
  lcd.print("Status");
  lcd.setCursor(10, 2);
  lcd.print("Arcus");
  lcd.setCursor(10, 3);
  lcd.print("Module");

  
  Votagemeasure();     // Measuring Voltage program for Battery
  Currentmeasure();    // Measuring Current from battery output
  LightPro();          // LED Program to show status of module
  CurrentControl();    // Current Program to adjust duty cycle of PWM
  Status();            // Program to display Status of module on LCD Screen
  VoltDis();           // Program to display Voltage of battery on LCD Screen
  AmpDis();            // Program to display Current going to battery/load on LCD Screen
}
//-----------------------------------------LOOP-------------------------------------------------//
//---------------------------------------LightPro-------------------------------------------//
void LightPro()
{
  // LED Program when Battery is full
  // Green LED is ON
  if (Batfull) {
    digitalWrite(26, LOW);
    digitalWrite(27, HIGH);
    digitalWrite(14, LOW);
  }
  // LED Program when Battery is Charging
  // Blue LED is Flashing
  else if (Batcharge) {
    digitalWrite(27, LOW);
    digitalWrite(14, LOW);
    if (Timer1 == 0) {
      digitalWrite(26, HIGH);
      TS1 = 1;
    }
    else if (Timer1 == 10) {
      digitalWrite(26, LOW);
      TS1 = 0;
    }
    if (TS1 == 1) {
      Timer1 = Timer1 + 1;
    }
    if (TS1 == 0) {
      Timer1 = Timer1 - 1;
    }
  }
  // LED Program if no battery or wrong battery charge
  // Red LED is Flashing
  else if (Baterror) {
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    if (Timer2 == 0) {
      digitalWrite(14, HIGH);
      TS2 = 1;
    }
    else if (Timer2 == 5) {
      digitalWrite(14, LOW);
      TS2 = 0;
    }
    if (TS2 == 1) {
      Timer2 = Timer2 + 1;
    }
    if (TS2 == 0) {
      Timer2 = Timer2 - 1;
    }
  }
}
//----------------------------------Votagemeasure------------------------------------------//
void Votagemeasure()
{
  // Voltage reading from Pin 4
  // Always measuring
  Voltage = analogRead(4);
  Voltage = (Voltage * 3.3) / 4095.0;
  Voltage = Voltage * 141.342 ;
}
//-------------------------------------VoltDis----------------------------------------------//
void VoltDis()
{
  // Displays Voltage to the LCD Screen 
  // Program to adjust the value position on LCD Screen
  if (Timer3 == 5) {
    if (Voltage != textv && Voltage < 10 || Startv == 0) {
      lcd.setCursor(1, 1);
      lcd.print(Voltage);
      textv = Voltage;
      Startv = 1;
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(5, 1);
      lcd.print("  ");
    }
    else if (Voltage != textv && Voltage >= 10) {

      lcd.setCursor(0, 1);
      lcd.print(Voltage);
      textv = Voltage;
    }
    else;
    Timer3 = 0;
  }
  else {
    Timer3 = Timer3 + 1;
  }
}
//----------------------------------Currentmeasure------------------------------------------//
void Currentmeasure()
{
  // Current reading from pin 38
  // Always measuring 
  Current = analogRead(38);
  Current = (Current * 3.3) / 4095.0;
  Current = Current * (10000.0 + 63500.0) / 63500.0;
  CVolt = Current; // CVolt show what the Voltage is at current reader
  Current = (Current - (1.91)) / 0.044;
}
//--------------------------------------AmpDis----------------------------------------------//
void AmpDis()
{
  // Displays Current to LCD Screen
  // Program to adjust the value position on LCD Screen
  if (Timer4 == 4) {
    if (Current != textv && Current < 10 && Current >= 0 || Startv == 0) {
      lcd.setCursor(1, 3);
      lcd.print(Current);
      lcd.setCursor(0, 3);
      lcd.print(" ");
      lcd.setCursor(5, 3);
      lcd.print(" ");
      textv = Current;
      Startv = 1;
    }
    else if (Current != textv && Current >= 10 || Current < 0) {
      lcd.setCursor(0, 3);
      lcd.print(Current);
      textv = Current;
      if (Current > 10) {
        lcd.setCursor(5, 3);
        lcd.print(" ");
      }
      else if (Current > -10 && Current < 0 ) {
        lcd.setCursor(5, 3);
        lcd.print(" ");
      }
    }
    else;
    Timer4 = 0;
  }
  else {
    Timer4 = Timer4 + 1;
  }
}
//---------------------------------CurrentControl-------------------------------------------//
void CurrentControl()
{
  // Current Control, to make Duty cycle change to make the output current to the set current
  // If theres is a battery to charge turn on the duty cycle
  if (Batcharge == 1) {
    // If Current is more then set current then bring Duty cycle down
    if (Current > (SetCurrent + 1) && dutyCycle >= 1)
    {
      if (Current > (SetCurrent + 5) && dutyCycle >= 1) {
        dutyCycle = dutyCycle - 5;
      }
      else {
        dutyCycle = dutyCycle - 1;
      }
      if (dutyCycle < 1) {
        dutyCycle = 1;
      }
    }
    // If Current is less then set current then bring Duty cycle up    
    else if (Current < SetCurrent && dutyCycle <= 254)
    {
      if (Current < (SetCurrent - 5) && dutyCycle <= 254) {
        dutyCycle = dutyCycle + 5;
      }
      else {
        dutyCycle = dutyCycle + 1;
      }
      if (dutyCycle > 254) {
        dutyCycle = 254;
      }
      if (dutyCycle < 1) {
        dutyCycle = 1;
      }
    }
    // If Current is equal to set current then keep there
    else if (Current >= SetCurrent && Current < (SetCurrent + 1))
    {
      ledcWrite(ledPin, dutyCycle);
    }
    else;
  }
  // If battery is full or battery error turn off the system
  else if (Baterror == 1 || Batfull == 1) {
    dutyCycle = 0;
  }
  ledcWrite(ledPin, dutyCycle);
}
//--------------------------------------Status----------------------------------------------//
void Status()
{
  // Status program to show and set the Status of the module
  // If the measured Voltage is equal or more then 400V the battery is full
  // The module is Off
  if (Voltage >= 400) {
    lcd.setCursor(10, 1);
    lcd.print("Done      ");
    Batfull = 1;
    Baterror = 0;
    Batcharge = 0;
  }
  // If the measured Voltage is in between 360V and 400V Set the module to charge the battery
  // The module is On 
  else if (Voltage < 400 && Voltage >= 360) {
    if ( Voltage > 390 && hys == 1);
    else if ( Voltage < 390 && hys == 1) {
      hys = 0;
    }
    else {
      Batfull = 0;
      Baterror = 0;
      Batcharge = 1;
      if (Timer5 == 0) {
        lcd.setCursor(10, 1);
        lcd.print("Charging  ");
      }
      else if (Timer5 == 10) {
        lcd.setCursor(10, 1);
        lcd.print("Charging. ");
      }
      else if (Timer5 == 20) {
        lcd.setCursor(10, 1);
        lcd.print("Charging..");
      }
      else if (Timer5 == 30) {
        Timer5 = -1;
      }
      Timer5 = Timer5 + 1;
    }
  }
  // If the measured Voltage is less then 360V then set the module to error
  // The module is Off
  else {
    if (Timer6 == 0) {
      lcd.setCursor(10, 1);
      lcd.print("**ERROR** ");
    }
    else if (Timer6 == 10) {
      lcd.setCursor(10, 1);
      lcd.print("          ");
    }
    else if (Timer6 == 20) {
      lcd.setCursor(10, 1);
      lcd.print("*NOT*     ");
    }
    else if (Timer6 == 30) {
      lcd.setCursor(10, 1);
      lcd.print("*ACTIVE*  ");
    }
    else if (Timer6 == 40) {
      lcd.setCursor(10, 1);
      lcd.print("          ");
    }
    else if (Timer6 == 50) {
      lcd.setCursor(10, 1);
      lcd.print("**ERROR** ");
    }
    else if (Timer6 == 60) {
      lcd.setCursor(10, 1);
      lcd.print("          ");
    }
    else if (Timer6 == 70) {
      Timer6 = -1;
    }
    Timer6 = Timer6 + 1;
    Batfull = 0;
    Baterror = 1;
    Batcharge = 0;
    hys = 0;
  }
}
//---------------------------------------StartL---------------------------------------------//
void StartL()
{
  // Lighting program sequence to test LED's
  delay(100);
  digitalWrite(14, HIGH);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  delay(100);
  digitalWrite(14, LOW);
  digitalWrite(27, HIGH);
  digitalWrite(26, LOW);
  delay(100);
  digitalWrite(14, LOW);
  digitalWrite(27, LOW);
  digitalWrite(26, HIGH);
  delay(100);
  digitalWrite(14, HIGH);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  delay(100);
  digitalWrite(14, LOW);
  digitalWrite(27, HIGH);
  digitalWrite(26, LOW);
  delay(100);
  digitalWrite(14, LOW);
  digitalWrite(27, LOW);
  digitalWrite(26, HIGH);
  delay(100);
  digitalWrite(14, LOW);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  delay(100);
  digitalWrite(14, HIGH);
  digitalWrite(27, HIGH);
  digitalWrite(26, HIGH);
  delay(600);
  digitalWrite(14, LOW);
  digitalWrite(27, LOW);
  digitalWrite(26, LOW);
  delay(100);
}
