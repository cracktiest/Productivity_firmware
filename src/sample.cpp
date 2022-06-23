/*
  IP LISTING
  https://bit.ly/3bfRsU1

  THERE ARE 2 BOARDS, CHECK THE BOARD FIRST BEFORE UPLOAD:
  D1 R1 (RETIRED)
  [env:d1]
  platform = espressif8266
  board = d1
  _______________________________
  
  // #define PIN      D8 
  // int NUMPIXELS = 8;
  // int LED_B =D10;
  // const int IR_PIN =D5;

  D1 R2 MINI
  [env:d1_mini]
  platform = espressif8266
  board = d1_mini
  _______________________________
  // #define PIN      D5
  // int NUMPIXELS = 8;
  // int LED_B =D8;
  // const int IR_PIN =D0;

  PATCH 1.7
  + LCD MODE HAS BEEN CHANGED (OUTPUT / TARGET)
  + RGB LEDSTRIB HAS BEEN ADDED (OPTIMIZATION NEEDED)
  PATCH 1.6
  + EACH SQL QUERY ADDED DELAY 
  PATACH 1.5
  + LCD UI OPTIMIZATION
  PATCH 1.4 
  + DELAY BASED ON HOW LONG SERVER RESPONDS TO MICROCONTROLLER
  + lcd.clear() FUNCTION HAS BEEN ADDED AFTER SCANNING
  + OK BUTTON / TOMBOL10 HAS BEEN ADDED TWICE TO SCAN "BACADATA"
*/

#include <Arduino.h>
#include <rdm6300.h>
#include <ESP8266WiFi.h>
#include <TextFinder.h>
#include <IRremote.h>
#include <stdlib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN      D5
int NUMPIXELS = 8;
int LED_B =D8;
const int IR_PIN =D0;
int ts,h,mr,kw,bt,p,s,l,c=0;

WiFiClient client;
TextFinder  finder(client);
char webtext[25];
//koneksi wifi
const char* ssid     = "ROBOT";
const char* password = "robot8888";
const char* host ="10.5.0.204"; //10.5.0.108
const int port = 80;
int ulangclient;
String proses,out,target;
IPAddress local_IP(10,5,1,225);//249
IPAddress gateway(10,5,1,1);//(10,5,0,30);
IPAddress subnet(255,255,0,0);
char* id_device ="K6-100"; //41
char* link = "GM1";
String urlMode;
String urlID;
String urlText;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Rdm6300 rdm6300;
String tombol1 = "1";
String tombol2 = "2";
String tombol3 = "3";
String tombol4 = "4";
String tombol5 = "5";
String tombol6 = "6";
String tombol7 = "7";
String tombol8 = "8";
String tombol9 = "9";
String tombol10 = "10";

IRrecv irDetect(IR_PIN);
decode_results irIn;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void(* Reset) (void) = 0;

void warna(int R, int G, int B){
  pixels.clear();
  for(int i=0; i<8; i++){
  pixels.setBrightness(100);
  pixels.setPixelColor(i, pixels.Color(R,G,B));
  pixels.show();
  }
}

void beep (int bep){
  digitalWrite(LED_B, HIGH); 
  delay(bep);
  digitalWrite(LED_B, LOW);
  delay(bep);
}

void rgb(){
  if(ts>=1 && h==1 && proses == "Bekerja         "){
    warna(255,0,0);
  }
  else if(ts>=0 && h>=0 && proses == "Mesin Rusak     "){
    warna(0,0,255);
  }
  else if(ts>=0 && h>=0 && proses == "Kel. WIP        "){
    warna(155,0,155);
  }
  else if(ts>=0 && h>=0 && proses == "B. Teknis       "){
    warna(0,255,0);
  }
  else if(ts>=0 && h>=0 && proses == "Perbaikan       "){
    warna(155,120,0);
  }
  else if(ts>=0 && h>=0 && proses == "Mas. Supply     "){
    warna(255,128,0);
  }
  else if(ts>=0 && h>=0 && proses == "Kep Pribadi     "){
    warna(128,0,255);
  }
  else if(ts>=0 && h>=0 && proses == "P. Layout       "){
    warna(0,155,155);
  }
  else if(p>=5 || kw>=5 || bt>=5 || s>=5 || l>=5 ||c>=5){
    warna(0,255,0);
  }
  else if(mr>=5){
    warna(128,128,128);
  }
  else if(ts>=0 && h>=0 && "Selesai         "){
    warna(0,0,0);
  }
  else{
    warna(0,0,0);
  }
}

void bacadata(){
  if (!client.connect(host, port)) {
    Serial1.println("Server Unconnect");
    lcd.setCursor(0,0);
    lcd.print("SERVER UNCONNECT");
    beep(100);
    Reset();
  }
  urlText = "/robotik/monitoring/lihatdata.php?id_device=";
  urlText += id_device;
  client.print(String("GET ")+urlText+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
  ulangclient=0;       
  while(!client.available()){
    delay(1);
    ulangclient++;
    if(ulangclient>=5000){
      lcd.setCursor(0,0);
      lcd.print("SERVER UNRESPOND");
      lcd.setCursor(0,1);
      lcd.print("SILAHKAN  TUNGGU");
      Reset();
    }
  }
  Serial1.print("Latency reading from server : "); Serial1.print(ulangclient); Serial1.println(" ms");
  lcd.clear();
  while (client.available()){
  Serial1.println(urlText);
  if (finder.getString(id_device,"*",webtext,25) !=0){
    proses = webtext;
    lcd.setCursor(0,0);lcd.print("Set:");
    lcd.setCursor(4,0);lcd.print(proses);
    Serial1.print("Proses = "); Serial1.println(proses);
    }
  if (finder.getString("Output","^",webtext,25) !=0){
    out = webtext;
    lcd.setCursor(0,1);lcd.print("Out:");
    lcd.setCursor(4,1);lcd.print(out);
    lcd.print (' '); lcd.print('/'); lcd.print (' '); 
    Serial1.print("Output = "); Serial1.println(out);
    }
  if (finder.getString("Target","]",webtext,25) !=0){
    target = webtext;
//    lcd.setCursor(9,1);lcd.print("Tar:");
//    lcd.setCursor(13,1);
    lcd.print(target); 
    Serial1.print("Target = "); Serial1.println(target);
    }
    //________
    
     if (finder.getString("TS","!",webtext,25) !=0){
      String tapspv = webtext;
      ts = tapspv.toInt();
      Serial1.print("Jam Tap = "); Serial1.println(ts);
    }
     if (finder.getString("H","@",webtext,25) !=0){
      String hasil = webtext;
      h = hasil.toInt();
      Serial1.print("Hasil = "); Serial1.println(h);
    }
     if (finder.getString("M","#",webtext,25) !=0){
      String mesin = webtext;
      mr = mesin.toInt();
      Serial1.print("M.Rusak = "); Serial1.println(mr);
    }    
     if (finder.getString("K","%",webtext,25) !=0){
      String kelebihan = webtext;
      kw = kelebihan.toInt();
      Serial1.print("K.WIP = "); Serial1.println(kw);
    }    
     if (finder.getString("B","&",webtext,25) !=0){
      String bantuan = webtext;
      bt = bantuan.toInt();
  Serial1.print("B.Teknis = "); Serial1.println(bt);
    }        
     if (finder.getString("P","(",webtext,25) !=0){
      String perbaikan = webtext;
      p = perbaikan.toInt();
  Serial1.print("Perbaikan = "); Serial1.println(p);
    }            
     if (finder.getString("S",")",webtext,25) !=0){
      String masalah = webtext;
      s = masalah.toInt();
  Serial1.print("M.Suplay = "); Serial1.println(s);
    }
     if (finder.getString("L","[",webtext,25) !=0){
      String lost = webtext;
      l = lost.toInt();
  Serial1.print("Pribadi = "); Serial1.println(l);
    }                           
     if (finder.getString("C","]",webtext,25) !=0){
      String change = webtext;
      c = change.toInt();
  Serial1.print("P.Layout = "); Serial1.println(c);
    } 
  
  // if (finder.getString("fsl","]",webtext,25) !=0){
  //   String fasilitas = webtext;
  //   Serial1.print("Fasilitas = "); Serial1.println(fasilitas);}
  // if (finder.getString("spv","]",webtext,25) !=0){
  //   String spv = webtext;
  //   Serial1.print("SPV = "); Serial1.println(spv);}
  }    
  // proses,out,target="0";
  // webtext=0;
  rgb();
}

void decodeIR() // Indicate what key is pressed
{ 
  switch (irIn.value) { // compare the value to the following cases
    case 0xFFA25D: // if the value is equal to 0xFD00FF
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol1;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }        
      bacadata();
      break;
    case 0xFF629D:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol2;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }        
      bacadata();
      break;
    case 0xFFE21D:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol3;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
        } 
      bacadata();
      break;
    case 0xFF22DD:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol4;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode); 
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break;
    case 0xFF02FD:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol5;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);      
      } 
      bacadata();
      break ;
    case 0xFFC23D:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol6;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);      
        }
      bacadata();
      break ;
    case 0xFFE01F:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol7;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);      
      } 
      bacadata();
      break ;
    case 0xFFA857:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol8;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
        } 
      bacadata();
      break ;
    case 0xFF906F:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol9;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);        
        bacadata;
        Serial1.println(client.readString());
      } 
      bacadata();
      break;
    case 0xFF38C7:
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol10;
      urlMode +="&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ")+urlMode+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");
        Serial.println(urlMode);  
        ulangclient=0;
        while(!client.available()){
          delay(1);
          ulangclient++;
          if(ulangclient>=5000){
            lcd.setCursor(0,0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0,1);
            lcd.print("SILAHKAN  TUNGGU");
            Reset();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient); 
        Serial1.println(client.readString());       
      } 
      bacadata();
      break ; 
    case 0xFF6897:
      lcd.setCursor(0,0); lcd.print(" PROGRAM  RESET ");
      beep(500);
      Reset();
      break;
    case 0xFFB04F:
      beep(500);
      return setup();
      break;
      default:
      beep(200);
      bacadata();
      break;
  }
}

void rfid(){
  if (rdm6300.update()){
    beep(100);
    String getID= String(rdm6300.get_tag_id(), HEX);
    Serial1.println(getID);
    digitalWrite(READ_LED_PIN, rdm6300.is_tag_near());
    delay(10);
    urlID = "/robotik/monitoring/getID.php?nokartu=" ;
    urlID += getID;
    urlID +="&id_device=";
    urlID += id_device;
    urlID += "&link=";
    urlID += link;   
  if (!client.connect(host, port)) {
    Serial1.println("Server Unconnect");
    lcd.setCursor(0,0);
    lcd.print("SERVER UNCONNECT");
    beep(100);
    Reset();
  }
  if (client.connect(host, port)) {
    client.print(String("GET ")+urlID+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");  
    Serial1.println(urlID);
    // delay(100);  
    ulangclient = 0;
        // while(!client.available()){
        //   delay(1);
        //   ulangclient++;
        //   if(ulangclient>=5000){
        //     lcd.setCursor(0,0);
        //     lcd.print("SERVER UNRESPOND");
        //     lcd.setCursor(0,1);
        //     lcd.print("SILAHKAN  TUNGGU");
        //     Reset();
        //   }
        // }
  }
  // Serial1.print("Latency from writing rfid to server : "); Serial1.println(ulangclient);      
  rdm6300.end();
  rdm6300.begin(RDM6300_RX_PIN);
  bacadata();
  } 
}
void setup() {
  pixels.begin();
  pixels.clear();
  Serial1.begin(9600);
  Serial1.print("Connecting to ");
  Serial1.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(READ_LED_PIN, OUTPUT);
  digitalWrite(READ_LED_PIN, LOW);
  pinMode(LED_B, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  irDetect.enableIRIn(); 
  rdm6300.begin(RDM6300_RX_PIN);
  WiFi.config(local_IP,gateway,subnet);
  while (WiFi.status() != WL_CONNECTED) {
    beep(50);
    Serial1.print(".");
    lcd.setCursor(0,0);
    lcd.print("Wifi Connecting.");
  }
  Serial1.println("");
  Serial1.println("WiFi connected.");
  Serial1.println("IP address: ");
  Serial1.println(WiFi.localIP());
  lcd.setCursor(0,0);
  lcd.print(" Wifi connected ");
  warna(0,0,0);
  bacadata();
}
void loop() {
  if (irDetect.decode(&irIn)) 
  {
    decodeIR(); 
    IrReceiver.resume(); 
  }
  rfid();
}

