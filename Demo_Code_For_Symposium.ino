//-----------------------------------------------------------------------------------------------------------------//
// This Program is made for the Altspark Final Project, this code will go into the esp32-devkitm-1
// and well control the DC-DC Voltage Covertor to charge a 400VDC Leaf Battery.
// Will need to download the esp32 by espressif systems in board manager and set to ESP32 Wrover kit (all version).
// Will also download the Liquidcrystal I2C from Library Manager.
//
//By Kongphop Amphandang
//Date Oct/15/2024
//-----------------------------------------------------------------------------------------------------------------//
// LCD Display Library
#include <LiquidCrystal.h>

// The number of the PWM pin
const int ledPin = 4;

// Setting LCD Sreen Pin
const int rs = 5, en = 18, d4 = 23, d5 = 19, d6 = 22, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Setting PWM properties
const int freq = 60000;
const int resolution = 8;
float dutyCycle = 0; //0-255 DutyCycle
float Voltage = 0;

// DutyCycle text setup
int dutyCycletext = 0;
int text = 0;
int Start = 0;
float textv = 0;
float Startv = 0;

// Screen Move
int Move = 1;
int Stop = 0;
int Clear = 0;
int Stope = 0;

// Voltage Setting
int V = 15;
int C = 4095 / V;

//Current setting
float Current = 15;
float CVolt = 0;              // Voltage at current reader
float dutyCycleD = 0;

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

//------------------------------------------------------------------------------------------//
void setup() {

  // Pinmodes
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(38, INPUT);
  pinMode(37, INPUT);
  pinMode(36, INPUT);


  // Configure LED PWM
  ledcAttach(ledPin, freq, resolution);

  // Setting Team Name for startup
  lcd.begin(16, 2);
  lcd.print("AltSpark Tech");
  lcd.setCursor(0, 1);
  lcd.print("ARCUS Module");
  // LED Test On
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(25, HIGH);

  delay(2000);

  // LED Test Off
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(25, LOW);

  // LED Test sequence Program
  StartL();

  // Clear Screen
  lcd.clear();
}


//------------------------------------------------------------------------------------------//
//-----------------------------------------LOOP-------------------------------------------------//
void loop() {

  CurrentM();
  DiffScene();
  LightPro();
  Votagemeasure();
  CurrentControl();
  Status();

  if (Clear != Move && Stope == 0) {
    lcd.clear();
    Clear = Move;
    Stope = 1;
  }
  else if (digitalRead(36) == LOW && digitalRead(38) == LOW) {
    Stope = 0;
    Stop = 0;
  }

  if (Move == 1) {
    StatusDis();
    delay(2);
  }
  else if (Move == 2) {
    VoltDis();
    delay(2);
  }
  else if (Move == 3) {
    CurrentDisplay();
    delay(2);
  }
  else if (Move == 4) {
    DutycycDis();
    delay(2);
  }
  else if (Move == 5) {
    DisRunTime();
  }
  else if (Move == 6) {
    Projectname();
  }
  else;

}
//-----------------------------------------LOOP-------------------------------------------------//
//------------------------------------------------------------------------------------------//
void DutycycDis()
{
  lcd.setCursor(0, 0);

  lcd.print("Duty Cycle");

  dutyCycleD = dutyCycle / 255;
  dutyCycleD = dutyCycleD * 100;

  if (dutyCycleD != textv && dutyCycleD < 10 && dutyCycleD >= 0 || Startv == 0) {
    lcd.setCursor(1, 1);
    lcd.print(dutyCycleD);
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(2, 1);
    //  lcd.print("   ");
    textv = dutyCycleD;
    Startv = 1;
  }
  else if (dutyCycleD != textv && dutyCycleD >= 10 || dutyCycleD < 0) {
    lcd.setCursor(0, 2);
    lcd.print(dutyCycleD);
    textv = dutyCycleD;
    if (dutyCycle > 10) {
      lcd.setCursor(5, 2);
      // lcd.print("   ");
    }
    else if (dutyCycleD > -10 && dutyCycleD < 0 ) {
      lcd.setCursor(2, 2);
      // lcd.print("   ");
    }
  }
}
//------------------------------------------------------------------------------------------//
void CurrentM()
{
  // Current reading from pin 38
  // Always measuring
  Current = analogRead(35);
  Current = Current / 163.8;
  //Current = (Current * 3.3) / 4095.0;
  //  Current = Current * (10000.0 + 63500.0) / 63500.0;
  //  CVolt = Current; // CVolt show what the Voltage is at current reader
  //  Current = (Current - (1.91)) / 0.044;
}

//------------------------------------------------------------------------------------------//
void CurrentDisplay()
{

  lcd.setCursor(0, 0);

  lcd.print("Current");


  // Displays Current to LCD Screen
  // Program to adjust the value position on LCD Screen
  if (Timer4 == 4) {
    if (Current != textv && Current < 10 && Current >= 0 || Startv == 0) {
      lcd.setCursor(1, 1);
      lcd.print(Current);
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(5, 1);
      lcd.print("   ");
      textv = Current;
      Startv = 1;
    }
    else if (Current != textv && Current >= 10 || Current < 0) {
      lcd.setCursor(0, 1);
      lcd.print(Current);
      textv = Current;
      if (Current > 10) {
        lcd.setCursor(5, 1);
        lcd.print("    ");
      }
      else if (Current > -10 && Current < 0 ) {
        lcd.setCursor(2, 1);
        lcd.print("   ");
      }
    }
    else;
    Timer4 = 0;
  }
  else {
    Timer4 = Timer4 + 1;
  }

}
//------------------------------------------------------------------------------------------//
void LightPro()
{
  // LED Program when Battery is full
  // Green LED is ON
  if (Batfull) {
    digitalWrite(32, LOW);
    digitalWrite(33, HIGH);
    digitalWrite(25, LOW);
  }
  // LED Program when Battery is Charging
  // Blue LED is Flashing
  else if (Batcharge) {
    digitalWrite(33, LOW);
    digitalWrite(25, LOW);
    if (Timer1 == 0) {
      digitalWrite(32, HIGH);
      TS1 = 1;
    }
    else if (Timer1 == 100) {
      digitalWrite(32, LOW);
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
    digitalWrite(32, LOW);
    digitalWrite(33, LOW);
    if (Timer2 == 0) {
      digitalWrite(25, HIGH);
      TS2 = 1;
    }
    else if (Timer2 == 50) {
      digitalWrite(25, LOW);
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
//------------------------------------------------------------------------------------------//
void StatusDis()
{
  lcd.setCursor(0, 0);
  lcd.print("Status");

  if (Batfull) {
    lcd.setCursor(0, 1);
    lcd.print("Done      ");
  }

  else if (Batcharge) {
    if (Timer5 == 0) {
      lcd.setCursor(0, 1);
      lcd.print("Charging  ");
    }
    else if (Timer5 == 100) {
      lcd.setCursor(0, 1);
      lcd.print("Charging. ");
    }
    else if (Timer5 == 200) {
      lcd.setCursor(0, 1);
      lcd.print("Charging..");
    }
    else if (Timer5 == 300) {
      Timer5 = -1;
    }
    Timer5 = Timer5 + 1;
  }
  else if (Baterror) {
    if (Timer6 == 0) {
      lcd.setCursor(0, 1);
      lcd.print("**ERROR** ");
    }
    else if (Timer6 == 200) {
      lcd.setCursor(0, 1);
      lcd.print("          ");
    }
    else if (Timer6 == 400) {
      lcd.setCursor(0, 1);
      lcd.print("*NOT*     ");
    }
    else if (Timer6 == 600) {
      lcd.setCursor(0, 1);
      lcd.print("*ACTIVE*  ");
    }
    else if (Timer6 == 800) {
      lcd.setCursor(0, 1);
      lcd.print("          ");
    }
    else if (Timer6 == 1000) {
      lcd.setCursor(0, 1);
      lcd.print("**ERROR** ");
    }
    else if (Timer6 == 1200) {
      lcd.setCursor(0, 1);
      lcd.print("          ");
    }
    else if (Timer6 == 1400) {
      Timer6 = -1;
    }
    Timer6 = Timer6 + 1;
  }









}
//------------------------------------------------------------------------------------------//
void DisRunTime()
{


  lcd.setCursor(0, 0);
  lcd.print("Run Time");
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);

}
//------------------------------------------------------------------------------------------//
void DiffScene()
{

  if (digitalRead(36) == HIGH && Stop != 1) {

    Move = Move + 1;
    Stop = 1;
  }
  else if (digitalRead(38) == HIGH && Stop != 1) {
    Move = Move - 1;
    Stop = 1;
  }
  else;

  if (Move > 6) {
    Move = 1;
  }
  else if (Move < 1) {
    Move = 6;
  }

}
//------------------------------------------------------------------------------------------//
void Projectname()
{


  lcd.setCursor(0, 0);
  lcd.print("AltSpark Tech");
  lcd.setCursor(0, 1);
  lcd.print("Voltage Module");


}
//------------------------------------------------------------------------------------------//
void Votagemeasure()
{
  Voltage = analogRead(34);
  Voltage = (Voltage * 3.3) / 4095.0;
  Voltage = Voltage * 141.342 ;



}
//------------------------------------------------------------------------------------------//
void VoltDis()
{

  lcd.setCursor(0, 0);

  lcd.print("Voltage");



  // Displays Voltage to the LCD Screen
  // Program to adjust the value position on LCD Screen
  if (Timer3 == 200) {
    if (Voltage != textv && Voltage < 10 || Startv == 0) {
      lcd.setCursor(1, 1);
      lcd.print(Voltage);
      textv = Voltage;
      Startv = 1;
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.setCursor(5, 1);
      lcd.print("   ");
    }
    else if (Voltage != textv && Voltage >= 10) {

      lcd.setCursor(0, 1);
      lcd.print(Voltage);
      if (Voltage < 100) {
        lcd.setCursor(5, 1);
        lcd.print("   ");
      }
      else if (Voltage >= 100) {
        lcd.setCursor(6, 1);
        lcd.print("   ");
      }
      textv = Voltage;
    }
    else;
    Timer3 = 0;
  }
  else {
    Timer3 = Timer3 + 1;
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
  delay(2);
}
//--------------------------------------Status----------------------------------------------//
void Status()
{
  // Status program to show and set the Status of the module
  // If the measured Voltage is equal or more then 400V the battery is full
  // The module is Off
  if (Voltage >= 400) {

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

    }
  }
  // If the measured Voltage is less then 360V then set the module to error
  // The module is Off
  else {

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
  digitalWrite(25, HIGH);
  digitalWrite(33, LOW);
  digitalWrite(32, LOW);
  delay(100);
  digitalWrite(25, LOW);
  digitalWrite(33, HIGH);
  digitalWrite(32, LOW);
  delay(100);
  digitalWrite(25, LOW);
  digitalWrite(33, LOW);
  digitalWrite(32, HIGH);
  delay(100);
  digitalWrite(25, HIGH);
  digitalWrite(33, LOW);
  digitalWrite(32, LOW);
  delay(100);
  digitalWrite(25, LOW);
  digitalWrite(33, HIGH);
  digitalWrite(32, LOW);
  delay(100);
  digitalWrite(25, LOW);
  digitalWrite(33, LOW);
  digitalWrite(32, HIGH);
  delay(100);
  digitalWrite(25, LOW);
  digitalWrite(33, LOW);
  digitalWrite(32, LOW);
  delay(100);
  digitalWrite(25, HIGH);
  digitalWrite(33, HIGH);
  digitalWrite(32, HIGH);
  delay(600);
  digitalWrite(25, LOW);
  digitalWrite(33, LOW);
  digitalWrite(32, LOW);
  delay(100);
}
