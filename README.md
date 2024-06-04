## SERIAL DRIVER
+ [FT 232] (https://www.ftdichip.com/old2020/Drivers/VCP.htm)
+ [CH 340] (https://sparks.gogo.co.nz/ch340.html)

## [IP LOKAL ROBOTIK](https://bit.ly/3bfRsU1)

##  THERE ARE 2 BOARDS, CHECK THE BOARD FIRST BEFORE UPLOAD:
###  D1 R1 (RETIRED)
+  [env:d1]
+  platform = espressif8266
+  board = d1
  _______________________________
  
+  // #define PIN      D8 
+  // int NUMPIXELS = 8;
+  // int LED_B =D10;
+  // const int IR_PIN =D5;

###  D1 R2 MINI
+  [env:d1_mini]
+  platform = espressif8266
+  board = d1_mini
  _______________________________
+  // #define PIN      D5
+  // int NUMPIXELS = 8;
+  // int LED_B =D8;
+  // const int IR_PIN =D0;


## BUG
WiFi disconnected after analog pin is used, will fix later. Even ESP community can't fix this bug 
![image](https://github.com/cracktiest/PerOP-Operator/assets/104487851/4a29154b-5c4a-4743-bda3-ca5eb43eb600)


## LOG CHANGED

### PATCH 1.8
+ More stable program
+ ESP8266MultiWifi function has been added
+ Bundle program has been added
+ Limit switch sensor has been added to detect hanger's movement 
+ Will catch more bug

### PATCH 1.7
+ LCD mode has been changed (Output/Target)
+ RGB Ledstrip has been added (Optimization needed asap)

### PATCH 1.6 
+ Each SQL Query added delay++ function

### PATCH 1.5 
  + Minimize microcontroller from freezing after fail reading to server "bacadata function"
  + Unused finder.getString (s) are commented
                                                                                                                                                                         
###  PATCH 1.4
  + DELAY BASED ON HOW LONG SERVER RESPONDS TO MICROCONTROLLER
  + lcd.clear() FUNCTION HAS BEEN ADDED AFTER SCANNING 
  + OK BUTTON / TOMBOL10 HAS BEEN ADDED TWICE TO SCAN "BACADATA"
  
## LIBRARY
  [RDM 6300](https://github.com/arduino12/rdm6300)
  
  [Text Finder](https://github.com/tardate/TextFinder)
 
 [LCD_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)
 
