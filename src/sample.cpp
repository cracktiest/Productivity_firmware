/*
  PATCH 1.4 
  FEATURE CHANGED :
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
//lib
WiFiClient client;
TextFinder  finder(client);
char webtext[25];
//koneksi wifi
const char* ssid     = "ROBOT V2";
const char* password = "robot%8888";
const char* host ="10.5.0.138"; //10.5.0.108
const int port = 80;
String proses,out,target;

//setting ip 
IPAddress local_IP(10,5,1,222);//249
IPAddress gateway(10,5,1,1);//(10,5,0,30);
IPAddress subnet(255,255,0,0);
char* id_device ="K6-1";;
char* link = "GM1";
String urlMode;
String urlID;
String urlText;

Rdm6300 rdm6300;
int LED_B =D8;
const int IR_PIN =D0;
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

void beep (int bep){
  digitalWrite(LED_B, HIGH); 
  delay(bep);
  digitalWrite(LED_B, LOW);
  delay(bep);
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
  int ulangclient=0;       
  while(!client.available()){
    delay(1);
    ulangclient++;
    if(ulangclient>=2000){
      lcd.setCursor(0,0);
      lcd.print("SERVER UNRESPOND");
      lcd.setCursor(0,1);
      lcd.print("SILAHKAN  TUNGGU");
      Reset();
    }
  }
  Serial1.print("Lamanya delay "); Serial1.print(ulangclient); Serial1.println(" ms");
  while (client.available()){
  Serial1.println(urlText);
  if (finder.getString(id_device,"*",webtext,25) !=0){
    lcd.clear();
    String proses = webtext;
    lcd.setCursor(0,0);lcd.print("Set:");
    lcd.setCursor(4,0);lcd.print(proses);
    Serial1.print("Proses = "); Serial1.println(proses);
    }
  if (finder.getString("Output","^",webtext,25) !=0){
    String out = webtext;
    lcd.setCursor(0,1);lcd.print("Out:");
    lcd.setCursor(4,1);lcd.print(out);
    Serial1.print("Output = "); Serial1.println(out);
    }
  if (finder.getString("Target","]",webtext,25) !=0){
    String target = webtext;
    Serial1.print("Target = "); Serial1.println(target);}
  // if (finder.getString("fsl","]",webtext,25) !=0){
  //   String fasilitas = webtext;
  //   Serial1.print("Fasilitas = "); Serial1.println(fasilitas);}
  // if (finder.getString("spv","]",webtext,25) !=0){
  //   String spv = webtext;
  //   Serial1.print("SPV = "); Serial1.println(spv);}
  }    
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
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
        delay(100);
        bacadata();
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
  if (client.connect(host, port)) {
    client.print(String("GET ")+urlID+" HTTP/1.1\r\n" +"Host: "+host+ "\r\n" +"Connection: close\r\n\r\n");  
    Serial1.println(urlID);
    delay(100);  
    }    
  rdm6300.end();
  rdm6300.begin(RDM6300_RX_PIN);
  bacadata();
  }
  
}
void setup() {
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
