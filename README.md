 # ARCUS-Program
This Program is made for the Altspark Arcus Module for the 2024 Camosun Capstone Project,
this code will go into the esp32-devkitm-1 that will control the AC-DC Voltage Covertor
to charge a 400VDC Leaf Battery.

To program the Esp32 you'll have to download a library and a board library.
The Library you need to download: LiquidCrystal I2C By Marco Schwartz.
You can find this in the Manage Library tab under Tools on the top of the Arduino IDE.
The Board Library you need to download: esp32 By Espressif Systems.
You can find this in the Board Manager tab under Tools on the top of the Arduino IDE.

(If your downloading from the IDE the esp32 board Library might take a while to download 
if the download gets stuck on downloading the tools for a while then you might want to 
end process by ending it in task manager and try again. It might take a while until one
download works)

This program was programed on the Arduino Legacy IDE.

We have 3 files in this project file
-Final_program
-Test_Code_for_Current_Sensor
-Demo_Code_For_Symposium
The Final_program is the program that you should use for the normal operations of the module.
The Test_Code_for_Current_Sensor is the code you should before the Final_program to get the current sensor zero voltage.
(BE AWARE that in Test_Code_for_Current_Sensor The Duty Cycle of the PWM is set to a non zero amount so if you don't want output then set Duty Cycle to 0)
The Demo_Code_For_Symposium is the program we use for the Symposium so it have some extra programs the Final_program does not to Account for the smaller screen.

By Kongphop Amphandang
