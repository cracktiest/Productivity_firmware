/*


  05 JUNI 202
  FEATURE CHANGED :
  + TOO MUCH

  this program is only for D1R1 board type
*/
//#define DEBUG_ESP_PORT Serial

#include <Arduino.h>
#include <rdm6300.h>
//#include <WiFiUdp.h>
//#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <TextFinder.h>
#include <IRremote.hpp>
#include <stdlib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define RDM6300_RX_PIN 13 // can be only 13 - on esp8266 force hardware uart!
#define READ_LED_PIN 16
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN D8 //LED RGBSTRIP

//unsigned long starmilis, curenmilis;

unsigned long starmilis, curenmilis;
const unsigned long periode = 5000;
//const unsigned long periode =5000;
ESP8266WiFiMulti wifi_multi;
const char* ssid1     = "ROBOT V1";
const char* ssid2     = "ROBOT V2";
const char* ssid3     = "ROBOT V3";
const char* ssid4     = "ROBOT V4";
const char* ssid5     = "ROBOT V5";
const char* ssid6     = "ROBOT V6";
const char* password = "robot%8888";
const char* host = "10.5.2.222"; //10.5.0.108
const int port = 80;
uint16_t connectTimeOutPerAP = 5000; //Defines the TimeOut(ms) which will be used to try and connect with any specific Access Point

int limitSwitch = A0;
int increment = 1; //penambahan ketika sensor limit switch terdeteksi
int NUMPIXELS = 8;
int LED_B = D10;
int remot;
int a, tapHanger = 0;
const int IR_PIN = D5;
int ts, h, mr, kw, bt, p, s, l, c = 0;
String getID,targetShow;
WiFiClient client;
TextFinder  finder(client);
char webtext[25];
int ulangclient, ulanghost;
String proses, out, target;
int output;
//IPAddress local_IP(10,5,2,202); //249
//IPAddress gateway(10,5,2,1); //(10,5,0,30);
//IPAddress subnet(255,255,0,0);
char* id_device; //41
//char* id_device = "LF-33"; //41
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




void warna(int R, int G, int B) {
  pixels.clear();
  for (int i = 0; i < 8; i++) {
    pixels.setBrightness(100);
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.show();
  }
}

void beep (int bep) {
  digitalWrite(LED_B, HIGH);
  delay(bep);
  digitalWrite(LED_B, LOW);
  delay(bep);
}

void rgb() {
  if (ts >= 1 && h == 1 && proses == "Bekerja         ") {
    warna(255, 0, 0);
  }
  else if (ts >= 0 && h >= 0 && proses == "Mesin Rusak     ") {
    warna(0, 0, 255);
  }
  else if (ts >= 0 && h >= 0 && proses == "Kel. WIP        ") {
    warna(155, 0, 155);
  }
  else if (ts >= 0 && h >= 0 && proses == "B. Teknis       ") {
    warna(0, 255, 0);
  }
  else if (ts >= 0 && h >= 0 && proses == "Perbaikan       ") {
    warna(155, 120, 0);
  }
  else if (ts >= 0 && h >= 0 && proses == "Mas. Supply     ") {
    warna(255, 128, 0);
  }
  else if (ts >= 0 && h >= 0 && proses == "Kep Pribadi     ") {
    warna(128, 0, 255);
  }
  else if (ts >= 0 && h >= 0 && proses == "P. Layout       ") {
    warna(0, 155, 155);
  }
  else if (p >= 5 || kw >= 5 || bt >= 5 || s >= 5 || l >= 5 || c >= 5) {
    warna(0, 255, 0);
  }
  else if (mr >= 5) {
    warna(128, 128, 128);
  }
  else if (ts >= 0 && h >= 0 && "Selesai         ") {
    warna(0, 0, 0);
  }
  else {
    warna(0, 0, 0);
  }
}

void bacadata() {
  ulanghost = 0;

  lcd.setCursor(0, 0);
  lcd.print(" RECEIVING DATA ");
  while (!client.connect(host, port)) {
    delay(1);
    ulanghost++;
    if (ulanghost >= 5000) {
      Serial1.println("Server Unconnect | Bacadata");
      ulanghost = 0;
      wifi();
      //    lcd.setCursor(0, 0);
      //    lcd.print("SERVER UNCONNECT");
      //    lcd.setCursor(0, 1);
      //    lcd.print("  PLEASE  WAIT  ");
      //    beep(100);
      //    Reset();
    }
  }
  Serial1.print("Connect to server in : ");
  Serial1.println(String(ulanghost));
  urlText = "/robotik/monitoring/lihatdata.php?id_device=";
  urlText += id_device;
  client.print(String("GET ") + urlText + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  ulangclient = 0;
  while (!client.available()) {
    delay(1);
    ulangclient++;
    if (ulangclient >= 5000) {
      ulangclient = 0;
      wifi();
      //      lcd.setCursor(0, 0);
      //      lcd.print("SERVER UNRESPOND");
      //      lcd.setCursor(0, 1);
      //      lcd.print("SILAHKAN  TUNGGU");
      //      Reset();
    }
  }
  Serial1.print("Latency reading from server : "); Serial1.print(ulangclient); Serial1.println(" ms");
  lcd.clear();
  while (client.available()) {
    Serial1.println(urlText);
    if (finder.getString(id_device, "*", webtext, 25) != 0) {
      proses = webtext;
      lcd.setCursor(0, 0); lcd.print("Set:");
      lcd.setCursor(4, 0); lcd.print(proses);
      Serial1.print("Proses = "); Serial1.println(proses);
    }
    if (finder.getString("Output", "^", webtext, 25) != 0) {
      out = webtext;
      lcd.setCursor(0, 1); lcd.print("Out:");
      lcd.setCursor(4, 1); lcd.print(out);
      lcd.print (' '); lcd.print('/'); lcd.print (' ');
      Serial1.print("Output = "); Serial1.println(out);
      output = out.toInt();

    }
    if (finder.getString("Target", "]", webtext, 25) != 0) {
      target = webtext;
      targetShow = target;
      //    lcd.setCursor(9,1);lcd.print("Tar:");
      //    lcd.setCursor(13,1);
      lcd.print(target);
      Serial1.print("Target = "); Serial1.println(target);
    }
    //________

    if (finder.getString("TS", "!", webtext, 25) != 0) {
      String tapspv = webtext;
      ts = tapspv.toInt();
      Serial1.print("Jam Tap = "); Serial1.println(ts);
    }
    if (finder.getString("H", "@", webtext, 25) != 0) {
      String hasil = webtext;
      h = hasil.toInt();
      Serial1.print("Hasil = "); Serial1.println(h);
    }
    if (finder.getString("M", "#", webtext, 25) != 0) {
      String mesin = webtext;
      mr = mesin.toInt();
      Serial1.print("M.Rusak = "); Serial1.println(mr);
    }
    if (finder.getString("K", "%", webtext, 25) != 0) {
      String kelebihan = webtext;
      kw = kelebihan.toInt();
      Serial1.print("K.WIP = "); Serial1.println(kw);
    }
    if (finder.getString("B", "&", webtext, 25) != 0) {
      String bantuan = webtext;
      bt = bantuan.toInt();
      Serial1.print("B.Teknis = "); Serial1.println(bt);
    }
    if (finder.getString("P", "(", webtext, 25) != 0) {
      String perbaikan = webtext;
      p = perbaikan.toInt();
      Serial1.print("Perbaikan = "); Serial1.println(p);
    }
    if (finder.getString("S", ")", webtext, 25) != 0) {
      String masalah = webtext;
      s = masalah.toInt();
      Serial1.print("M.Suplay = "); Serial1.println(s);
    }
    if (finder.getString("L", "[", webtext, 25) != 0) {
      String lost = webtext;
      l = lost.toInt();
      Serial1.print("Pribadi = "); Serial1.println(l);
    }
    if (finder.getString("C", "]", webtext, 25) != 0) {
      String change = webtext;
      c = change.toInt();
      Serial1.print("P.Layout = "); Serial1.println(c);
    }
  }
  //  delay(3000); //agar bisa diping
  rgb();
  Serial1.println("Serial ended");
  client.stop();
  //  WiFi.forceSleepBegin();
  //  WiFi.disconnect();
  delay(1);
  beep(100);
  Serial1.print("Connected to ");
  Serial1.println(WiFi.SSID());
  Serial1.print("IP Address: ");
  Serial1.println(WiFi.localIP());
  Serial1.println("Wifi tewas");
}

void kirim(int nilai) {
  wifi();
  String urlSend;
  urlSend = "/robotik/monitoring/kirim.php?nokartu=" ;
  urlSend += getID;
  urlSend += "&id_device=";
  urlSend += id_device;
  urlSend += "&link=";
  urlSend += link;
  urlSend += "&nilai=";
  urlSend += nilai;
  if (!client.connect(host, port)) {
    Serial1.println("Server Unconnect | RFID");
    lcd.setCursor(0, 0);
    beep(100);
    wifi();
  }
  if (client.connect(host, port)) {
    client.print(String("GET ") + urlSend + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
    Serial1.println(urlSend);
    // delay(100);
    ulangclient = 0;
    while (!client.available()) {
      delay(1);
      ulangclient++;
      if (ulangclient >= 5000) {
        lcd.setCursor(0, 0);
        lcd.print("SERVER UNRESPOND");
        lcd.setCursor(0, 1);
        lcd.print("SILAHKAN  TUNGGU");
        wifi();
        //          Reset();
      }
    }
    nilai = 0;
    output = 0;
  }
  client.stop();
  Serial1.print("Latency from writing rfid to server : "); Serial1.println(ulangclient);
  bacadata();
}

void decode_repeat(int lastcode) // Indicate what key is pressed
{
  switch (lastcode) {
    case 1:
      Serial1.println("Tombol tambah ditekan");
      if (proses == "Bekerja         ") {
        starmilis = curenmilis;
        getID = String(rdm6300.get_tag_id(), HEX);
        Serial1.println(getID);
        digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
        output++;
        Serial1.print("Hasilnya adalah :");
        Serial1.println(output);
        beep(100);
        delay(100);
        lcd.setCursor(0, 1); lcd.print("Out:");
        lcd.setCursor(4, 1); lcd.print(output);
        lcd.print (' '); lcd.print('/'); lcd.print (' ');
        lcd.setCursor(15, 1); lcd.print('~');
      }
      //      beep(200);
      break;
    case 2:
      Serial1.println("Tombol kurang pressed");
      if (proses == "Bekerja         ") {
        starmilis = curenmilis;
        getID = String(rdm6300.get_tag_id(), HEX);
        Serial1.println(getID);
        digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
        output--;
        Serial1.print("Hasilnya adalah :");
        Serial1.println(output);
        beep(100);
        delay(100);
        lcd.setCursor(0, 1); lcd.print("Out:");
        lcd.setCursor(4, 1); lcd.print(output);
        lcd.print (' '); lcd.print('/'); lcd.print (' ');
        lcd.setCursor(15, 1); lcd.print('~');
      }
      //      beep(200);
      break;
    //    case 3:
    //      Serial1.println("Tombol 3 pressed");
    //      beep(200);
    //      break;
    default:
      break;
  }
}
int nilaiakhir_ir = 0;

void decodeIR() // Indicate what key is pressed
{
  //  Serial.println(IrReceiver.printIRResultShort(&Serial));

  //  Serial1.println(IrReceiver.decodedIRData.decodedRawData);
  //  Serial1.println(IrReceiver.decodedIRData.decodedRawData);
  if (IrReceiver.decodedIRData.decodedRawData == 0) {
    Serial.println("TESS");
  }
  switch (IrReceiver.decodedIRData.decodedRawData) { // compare the value to the following cases
    case 3125149440: // if the value is equal to 0xFD00FF
      Serial1.println("Tombol 1 pressed");
      wifi();
      //      lcd.clear();
      //      lcd.setCursor(0, 0);
      //      lcd.print("SYNC DATA");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol1;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break;
    case 3108437760:
      Serial1.println("Tombol 2 pressed");
      wifi();
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol2;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }

        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break;
    case 3091726080:
      Serial1.println("Tombol 3 pressed");
      wifi();
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol3;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break;
    case 3141861120:
      wifi();
      Serial1.println("Tombol 4 pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol4;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break;
    case 3208707840:
      wifi();
      Serial1.println("Tombol 5 pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol5;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break ;
    case 3158572800:
      wifi();
      Serial1.println("Tombol 6 pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol6;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break ;
    case 4161273600:
      wifi();
      Serial1.println("Tombol 7 pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol7;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break ;
    case 3927310080:
      wifi();
      Serial1.println("Tombol 8 pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol8;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
      }
      bacadata();
      break ;
    case 4127850240:
      wifi();
      Serial1.println("Tombol 9 pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol9;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
        bacadata;
        delay(1000);
      }
      bacadata();
      break;
    case 3810328320:
      wifi();
      Serial1.println("Tombol OK pressed");
      beep(100);
      urlMode = "/robotik/monitoring/ubahmode.php?mode=";
      urlMode += tombol10;
      urlMode += "&id_device=";
      urlMode += id_device;
      urlMode += "&link=";
      urlMode += link;
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlMode + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial.println(urlMode);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            //            Reset();
            wifi();
          }
        }
        Serial1.print("Latency writing to server : "); Serial1.println(ulangclient);
        delay(1000);
      }
      bacadata();
      break ;

    case 3877175040:
      nilaiakhir_ir = 1;
      Serial1.println("Tombol tambah pressed");
      //        beep(100);
      if (proses == "Bekerja         ") {
        starmilis = curenmilis;
        getID = String(rdm6300.get_tag_id(), HEX);
        Serial1.println(getID);
        digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
        output++;
        Serial1.print("Hasilnya adalah :");
        Serial1.println(output);
        beep(100);
        delay(100);
        lcd.setCursor(0, 1); lcd.print("Out:");
        lcd.setCursor(4, 1); lcd.print(output);
        lcd.print (' '); lcd.print('/'); lcd.print (' ');
        lcd.setCursor(15, 1); lcd.print('~');
      }
      break;
    case 2907897600:
      nilaiakhir_ir = 2;
      Serial1.println("Tombol kurang pressed");
      //        beep(100);
      if (proses == "Bekerja         " && output > 1) {
        starmilis = curenmilis;
        getID = String(rdm6300.get_tag_id(), HEX);
        Serial1.println(getID);
        digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
        output--;
        Serial1.print("Hasilnya adalah :");
        Serial1.println(output);
        beep(100);
        delay(100);
        lcd.setCursor(0, 1); lcd.print("Out:");
        lcd.setCursor(4, 1); lcd.print(output);
        lcd.print (' '); lcd.print('/'); lcd.print (' ');
        lcd.setCursor(15, 1); lcd.print('~');
      }
      break;
    case 4061003520:
      if (proses == "Bekerja         " ) {
        lcd.clear();
        tapHanger = 0;
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0); lcd.print(" PROGRAM  RESET ");
        beep(500);
        Reset();
      }
      break;
    case 0:
      decode_repeat(nilaiakhir_ir);
      //    beep(200);
      break;
    case 3910598400:
      beep(500);
      ESP.restart();
//      return setup();
      break;
      //    default:
      //      beep(200);
      //      wifi();
      //      bacadata();
      //      break;
  }
}

void rfid() {
  if (rdm6300.get_tag_id()) {
    if (proses == "Bekerja         ") {
      starmilis = curenmilis;
      getID = String(rdm6300.get_tag_id(), HEX);
      Serial1.println(getID);
      digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
      output = output + tapHanger;
      Serial1.print("Hasilnya adalah :");
      Serial1.println(output);
      beep(100);
      delay(100);
      lcd.setCursor(0, 1); lcd.print("Out:");
      lcd.setCursor(4, 1); lcd.print(output);
      lcd.print (' '); lcd.print('/'); lcd.print (' ');
      lcd.setCursor(15, 1); lcd.print('~');
    }
    else {
      beep(100);
      String getID = String(rdm6300.get_tag_id(), HEX);
      Serial1.println(getID);
      digitalWrite(READ_LED_PIN, rdm6300.get_tag_id());
      delay(10);
      urlID = "/robotik/monitoring/getID.php?nokartu=" ;
      urlID += getID;
      urlID += "&id_device=";
      urlID += id_device;
      urlID += "&link=";
      urlID += link;
      wifi();
      if (!client.connect(host, port)) {
        Serial1.println("Server Unconnect | RFID");
        lcd.setCursor(0, 0);
        lcd.print("SERVER UNCONNECT");
        beep(100);
        wifi();
        //      Reset();
      }
      if (client.connect(host, port)) {
        client.print(String("GET ") + urlID + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
        Serial1.println(urlID);
        // delay(100);
        ulangclient = 0;
        while (!client.available()) {
          delay(1);
          ulangclient++;
          if (ulangclient >= 5000) {
            lcd.setCursor(0, 0);
            lcd.print("SERVER UNRESPOND");
            lcd.setCursor(0, 1);
            lcd.print("SILAHKAN  TUNGGU");
            wifi();
            //          Reset();
          }
        }
      }
      Serial1.print("Latency from writing rfid to server : "); Serial1.println(ulangclient);
      bacadata();
    }
    rdm6300.end();
    rdm6300.begin(RDM6300_RX_PIN);
  }
}

void wifi() {
  lcd.setCursor(0, 0);
  lcd.print("  CEK JARINGAN  ");
  //  lcd.setCursor(0,1);
  //  lcd.print("Silahkan  Tunggu");

//  wifi_multi.addAP(ssid1, password);
//  beep(10);
//  wifi_multi.addAP(ssid2, password);
//  beep(10);
//  wifi_multi.addAP(ssid3, password);
//  beep(10);
//  wifi_multi.addAP(ssid4, password);
//  beep(10);
//  wifi_multi.addAP(ssid5, password);
//  beep(10);
//  wifi_multi.addAP(ssid6, password);
//  beep(10);

  if (wifi_multi.run(connectTimeOutPerAP) != WL_CONNECTED) {

    beep(10);
    Serial1.print(".");
    delay(5000);
  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SSID : ");
    lcd.print(WiFi.SSID());
    lcd.setCursor(0, 1);
    lcd.print("IP   : ");
    lcd.print(WiFi.localIP());
    Serial1.print("Tersambung");
  }
  //  while (wifi_multi.run(connectTimeOutPerAP) != WL_CONNECTED)
  //  {
  //  }
  delay(1000);


  Serial1.println();
  Serial1.print("Connected to ");
  Serial1.println(WiFi.SSID());
  Serial1.print("IP Address: ");
  Serial1.println(WiFi.localIP());
  Serial1.print("MAC :");
  Serial1.println((char*)WiFi.macAddress().c_str());
  nilaiakhir_ir = 0;
}

void setup() {
  pixels.begin();
  pixels.clear();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial1.begin(115200);
  Serial1.print("Connecting to Wi-Fi. . .");
  //  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  lcd.setCursor(0, 0);
  lcd.print("    MEMULAI    ");
  lcd.print("  STARTING UP  ");
  lcd.setCursor(0, 1);
  lcd.print("Silahkan  Tunggu");

  wifi_multi.addAP(ssid1, password);
  beep(10);
  wifi_multi.addAP(ssid2, password);
  beep(10);
  wifi_multi.addAP(ssid3, password);
  beep(10);
  wifi_multi.addAP(ssid4, password);
  beep(10);
  wifi_multi.addAP(ssid5, password);
  beep(10);
  wifi_multi.addAP(ssid6, password);
  beep(10);

  while (wifi_multi.run(connectTimeOutPerAP) != WL_CONNECTED)
  {
    beep(10);
    Serial1.print(".");
    delay(5000);
  }
  delay(1000);

//  WiFi.printDiag(DEBUG_ESP_PORT);

  Serial1.println();
  Serial1.print("Connected to ");
  Serial1.println(WiFi.SSID());
  Serial1.print("IP Address: ");
  Serial1.println(WiFi.localIP());
  Serial1.print("MAC :");
  //change string to const char*
  Serial1.println((char*)WiFi.macAddress().c_str());

  //  WiFi.setAutoReconnect(true);
  //  WiFi.persistent(true);
  //convert string to char*
  String str = WiFi.macAddress();
  id_device = new char[str.length() + 1];
  str.toCharArray(id_device, str.length() + 1);

  pinMode(READ_LED_PIN, OUTPUT);
  digitalWrite(READ_LED_PIN, LOW);
  pinMode(LED_B, OUTPUT);
  irDetect.enableIRIn();
  rdm6300.begin(RDM6300_RX_PIN);

  lcd.setCursor(0, 0);
  lcd.print(" Wifi connected ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.SSID());

  warna(0, 0, 0);
  bacadata();

  starmilis = millis();
}
void hanger() {
  lcd.setCursor(0, 0);
  //  lcd.print(" TEST           ");
  lcd.print("INPUT BUNDLE   :");
  lcd.setCursor(0, 1);
  lcd.print("Bundle = ");
  lcd.print(a);
  lcd.print("               ");
}

void remotHanger() {
  beep(10);
  Serial1.println(IrReceiver.decodedIRData.decodedRawData);
  switch (IrReceiver.decodedIRData.decodedRawData) {
    case 3125149440:
      Serial1.println("Tombol 1 ditekan");
      if (a == 0) {
        a = 1;
      }
      else {
        a = (a * 10) + 1;
      }
      break;
    case 3108437760:
      Serial1.println("Tombol 2 ditekan");
      if (a == 0) {
        a = 2;
      }
      else {
        a = (a * 10) + 2;
      }
      break;
    case 3091726080:
      Serial1.println("Tombol 3 ditekan");
      if (a == 0) {
        a = 3;
      }
      else {
        a = (a * 10) + 3;
      }
      break;
    case 3141861120:
      Serial1.println("Tombol 4 ditekan");
      if (a == 0) {
        a = 4;
      }
      else {
        a = (a * 10) + 4;
      }
      break;
    case 3208707840:
      Serial1.println("Tombol 5 ditekan");
      if (a == 0) {
        a = 5;
      }
      else {
        a = (a * 10) + 5;
      }
      break;
    case 3158572800:
      Serial1.println("Tombol 6 ditekan");
      if (a == 0) {
        a = 6;
      }
      else {
        a = (a * 10) + 6;
      }
      break;
    case 4161273600:
      Serial1.println("Tombol 7 ditekan");
      if (a == 0) {
        a = 7;
      }
      else {
        a = (a * 10) + 7;
      }
      break;
    case 3927310080:
      Serial1.println("Tombol 8 ditekan");
      if (a == 0) {
        a = 8;
      }
      else {
        a = (a * 10) + 8;
      }
      break;
    case 4127850240:
      Serial1.println("Tombol 9 ditekan");
      if (a == 0) {
        a = 9;
      }
      else {
        a = (a * 10) + 9;
      }
      break;
    case 3860463360:
      Serial1.println("Tombol 0 ditekan");
      if (a == 0) {
        a = 0;
      }
      else {
        a = (a * 10) ;
      }
      break;
    case 3810328320:
      Serial1.println("Tombol OK ditekan");
      tapHanger = a;
      //      return();
      break;
    case 3877175040:
      Serial1.println("Tombol up ditekan");
      a++;
      break;
    case 2907897600:
      Serial1.println("Tombol down ditekan");
      a--;
      break;
    case 3910598400:
      Serial1.println("Reset alat");
      ESP.restart();
      break;
    case 4061003520:
      Serial1.println("Reset a");
      a = 0;
      break;
      //          case 0xFFA25D:
      //            Serial.println("Tombol 1 ditekan");
      //            break;

  }
  Serial1.println(a);
}


void loop() {
  if (proses == "Bekerja         " && tapHanger == 0) {
    target = out;
    hanger();
    if (IrReceiver.decode())
    {
      remotHanger();
      IrReceiver.resume();
    }
    if (tapHanger != 0) {

      lcd.setCursor(0, 0);
      lcd.print("Set:");
      lcd.setCursor(4, 0);
      lcd.print(proses);

      lcd.setCursor(0, 1);
      lcd.print("Out:");
      lcd.setCursor(4, 1);
      lcd.print(out);
      lcd.print(' ');
      lcd.print('/');
      lcd.print(' ');
      lcd.print(targetShow);
    }
    Serial.println("while lock");
  }
  else {
    if (digitalRead(D8) == HIGH) {
      beep(50);
      Serial1.println("HANGER TEDETEKSI");
      output = output + tapHanger;
      Serial1.println(output);
      kirim(output);
    }
    if (IrReceiver.decode())
    {
      decodeIR();
      IrReceiver.resume();
    }
    rfid();
    curenmilis = millis();
    if ((out.toInt() != output) && (curenmilis - starmilis > periode)) {
      Serial1.println("HEHE");
      kirim(output);
      starmilis = curenmilis;
      //        out=output;
      //      lcd.setCursor(0, 1); lcd.print("Out:");
      //      lcd.setCursor(4, 1); lcd.print(output);
      //      lcd.print (' '); lcd.print('/'); lcd.print (' ');
      //      lcd.setCursor(15,1); lcd.print(' ');
    }
  }
}
