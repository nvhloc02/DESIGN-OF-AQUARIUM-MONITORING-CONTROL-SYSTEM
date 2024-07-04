#include <FirebaseESP32.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h> 
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "time.h"
#include "icons.h"
#define FIREBASE_HOST "doan2-23ea3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "gDMQKxwjyhrpkRlqxtLXJOji2f0uSXLYU6pW8jxX"
FirebaseData fbdo;
const int sensorPin1 = 34;
#define ONE_WIRE_BUS 5
const int trig = 18; //Khai báo chân trig nối với chân số 8 là hằng số
const int echo = 19; //Khai báo chân echo nối với chân số 7 là hằng số
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
TFT_eSPI tft = TFT_eSPI(); 
#define relay1  25
#define relay2  26
#define relay3  27
#define relay4  14
float tempC;
char tempStr[8]; // Mảng để lưu trữ chuỗi số đã làm tròn
int sensorValue1; 
float voltage;
char volStr[8]; // Mảng để lưu trữ chuỗi số đã làm tròn
const char *ssid     = "SVK20";
const char *password = "0987654321";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ntpServer = "de.pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// Replace with your location Credentials
String lat = "10.833";
String lon = "106.667";;
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

int day;
int month;
int year;
int mode;
char ngay,a_ngay;
float a_tempC,a_voltage;
float Nguongnhiet,Nguongnuoc;
int value1,value2,value3;
int gio, a_gio;
int phut, a_phut;
int giay, a_giay;
int a_alarm_phut, b_alarm_phut, c_alarm_phut;
int a_alarm_gio, b_alarm_gio, c_alarm_gio;
//int alarm_phut = 0;
//int alarm_gio = 0;
long current;
struct tm timeinfo;
int h1,h2,h3,m1,m2,m3,amount1,feed, relay4State=0;
//int amount1, feed, relay3State=0;
unsigned long timing; //Thời gian là kiểu số nguyên
int distance, waterlevel, a_waterlevel;
unsigned long lastimetocontrol1 = 0,lastimetocontrol2 = 0;
const char* hourKeys[] = {"hour1", "hour2", "hour3"};
const char* minKeys[] = {"min1", "min2", "min3"};

// Define keys for Firebase
int hours[] = {h1, h2, h3};
int minutes[] = {m1, m2, m3};
int alarm_gio[] = {a_alarm_gio, b_alarm_gio, c_alarm_gio};
int alarm_phut[] = {a_alarm_phut, b_alarm_phut, c_alarm_phut};
void printLocalTime()
{
  getLocalTime(&timeinfo);
  tft.setTextSize(2);
  tft.setTextColor(TFT_SKYBLUE);
  tft.setCursor(0,0);
  tft.println(&timeinfo,"%a,%B %d,%Y");
}
void setup() {
  /* Setup debug console */
  Serial.begin(115200);
  sensors.begin();
  sensors.setResolution(12);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(trig, OUTPUT); //Chân trig xuất tín hiệu
  pinMode(echo, INPUT); //Chân echo nhận tín hiệu
  WiFi.begin(ssid, password);
  delay(100);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  timeClient.begin();
  tft.init();
  tft.setRotation(2);
  tft.invertDisplay( true );
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(15,10);
  tft.setTextSize(2);
  tft.setTextColor(TFT_RED);
  tft.println(F("THIET KE HE THONG"));
  tft.setCursor(10,30);
  tft.println(F("DIEU KHIEN GIAM SAT"));
  tft.setCursor(100,50);
  tft.println(F("BE CA"));
  for (int y = 0; y < 54; y++) 
  {
    for (int x = 0; x < 50; x++)
      {tft.drawPixel(100 + x, 90 + y,  SPKT [y * 50 + x]);
    }
  }
  tft.setTextSize(3);
  tft.setTextColor(TFT_BLUE);
  tft.setCursor(20,160);
  tft.println(F("SVTH"));
  tft.setTextSize(2);
  tft.setCursor(20,190);
  tft.println(F("TRAN MINH QUAN"));
  tft.setCursor(20,210);
  tft.println(F("NGUYEN VAN HUU LOC"));
  delay(7000);
  
  tft.fillScreen(TFT_BLACK);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  for (int y = 0; y < 20; y++) 
  {
    for (int x = 0; x < 20; x++)
      {tft.drawPixel(0 + x, 212 + y,  epd_bitmap_nhietdo[y * 20 + x]);
    }
  }

  for (int y = 0; y < 20; y++) 
  {
    for (int x = 0; x < 20; x++)
      {tft.drawPixel(0 + x, 183 + y,  epd_bitmap_doam[y * 20 + x]);
    }
  }
   for (int y = 0; y < 20; y++) 
  {
    for (int x = 0; x < 20; x++)
      {tft.drawPixel(0 + x, 154 + y,  water[y * 20 + x]);
    }
  }

  for (int y = 0; y < 54; y++) 
  {
    for (int x = 0; x < 50; x++)
      {tft.drawPixel(0 + x, 27 + y, epd_bitmap_spkt1[y * 50 + x]);
    }
  }

for (int y = 0; y < 54; y++) 
  {
    for (int x = 0; x < 54; x++)
      {tft.drawPixel(180 + x, 27 + y, epd_bitmap_dientu[y * 54 + x]);
    }
  }

  tft.setTextColor(TFT_PINK);
  tft.setTextSize(2);
  tft.setCursor(145,125);
  tft.println(F("Feeding"));
  tft.setCursor(25,125);
  tft.println(F("Sensor"));
  tft.setTextColor(TFT_PINK);
  tft.setTextSize(3);
  tft.setCursor(0,90);
  tft.println(F("Status:"));

  tft.fillRect(130,120 , 1,120 , TFT_BLUE);
  tft.fillRect(0,120 , 240,1 , TFT_BLUE);

  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(87,215);
  tft.println(F("C"));
  tft.setTextSize(1);
  tft.setCursor(83,210);
  tft.println(F("o"));
}

void feeding(){
    
    
    for (int i = 0; i < 3; i++) {
    // Check if hour or minute has changed
    if ((hours[i] != alarm_gio[i]) || (minutes[i] != alarm_phut[i])) {
        // Clear the area on the screen
        tft.fillRect(140, 155 + i * 30, 88, 22, TFT_BLACK);

        // Update alarm hours and minutes
        alarm_gio[i] = hours[i];
        alarm_phut[i] = minutes[i];

        // Format hours and minutes
        String setGio = (hours[i] < 10) ? "0" + String(hours[i]) : String(hours[i]);
        String setPhut = (minutes[i] < 10) ? "0" + String(minutes[i]) : String(minutes[i]);

        // Display the updated alarm time
        tft.setTextSize(3);
        tft.setTextColor(TFT_GREEN);
        tft.setCursor(140, 155 + i * 30);
        tft.println(String(setGio) + ":" + setPhut);
    }
  }
}
void loop() {
  if(millis() - lastimetocontrol1 >= 5000)
  {
   lastimetocontrol1 = millis();
   digitalWrite(trig,0); 
   delayMicroseconds(2); 
   digitalWrite(trig,1); 
   delayMicroseconds(10); 
   digitalWrite(trig,0);
   timing = pulseIn(echo, HIGH);
   distance = int(timing / 2 / 29.412); 
   waterlevel = 20 - distance;
   tft.setTextSize(2);
   tft.setTextColor(TFT_YELLOW);
   sensors.requestTemperatures();
   tempC = sensors.getTempCByIndex(0);
   if (tempC == -127.00 ){}
   else if(tempC == a_tempC){}
   else if(tempC != a_tempC) 
   {
    if(tempC < Nguongnhiet){
     dtostrf(tempC, 6, 2, tempStr); 
     Firebase.setFloat( fbdo,"Temperature", atof(tempStr));
     tft.fillRect(20,215 , 60,22 , TFT_BLACK);
     a_tempC == tempC;
     tft.setCursor(20,215);
     tft.println(atof(tempStr)); }
    else{
     tft.setTextColor(TFT_RED);
     dtostrf(tempC, 6, 2, tempStr); 
     Firebase.setFloat( fbdo,"Temperature", atof(tempStr));
     tft.fillRect(20,215 , 60,22 , TFT_BLACK);
     a_tempC == tempC;
     tft.setCursor(20,215);
     tft.println(atof(tempStr)); }
  }

  sensorValue1 = analogRead(sensorPin1); 
  voltage = 22 - (sensorValue1 *(5.0/1023.0));
  tft.setTextColor(TFT_YELLOW);
  if(voltage==a_voltage){}
  else{
    a_voltage==voltage;
    dtostrf(voltage, 6, 2, volStr); 
    Firebase.setFloat( fbdo,"Turbidity", atof(volStr));
    tft.setCursor(20,185);
    tft.fillRect(20, 185, 108, 22, TFT_BLACK);
    if (voltage <=10) {
    tft.println(F("Very Good"));
    } else if (voltage > 10 && voltage <= 20) {
    tft.println(F("Good"));
    } else if (voltage > 20) {
    tft.setTextColor(TFT_RED);
    tft.println(F("Medium"));
   }
  }
  tft.setTextColor(TFT_YELLOW);
  if((waterlevel==a_waterlevel) || (waterlevel == 20)){}
  else{
   a_waterlevel==waterlevel;
   Firebase.setInt(fbdo, "Waterlevel", waterlevel);
   tft.setCursor(20,155);
   if(waterlevel<Nguongnuoc) {tft.setTextColor(TFT_RED);tft.fillRect(20,155 , 100,22 , TFT_BLACK);tft.println(F("Low"));}
   else {tft.fillRect(20,155 , 100,22 , TFT_BLACK);tft.println(F("Normal"));}

   }
   Firebase.getInt(fbdo, "amount1");
   amount1 = fbdo.intData();
   Firebase.getInt(fbdo,"nguongnuoc");
   Nguongnuoc = fbdo.intData();
   Firebase.getInt(fbdo,"nguongnhiet");
   Nguongnhiet = fbdo.intData();
  }

  for (int i = 0; i < 3; i++) 
  {
    // Retrieve hour
    Firebase.getInt(fbdo, hourKeys[i]);
    hours[i] = fbdo.intData();
    
    // Retrieve minute
    Firebase.getInt(fbdo, minKeys[i]);
    minutes[i] = fbdo.intData();
  }
  feeding();
  

  timeClient.setTimeOffset(25200);
  timeClient.update();
  gio = timeClient.getHours();
  phut = timeClient.getMinutes();
  giay = timeClient.getSeconds();
  String chuoiGio = (gio < 10) ? "0" + String(gio) : String(gio);
  String chuoiPhut = (phut < 10) ? "0" + String(phut) : String(phut);
  //String chuoiGiay = (giay < 10) ? "0" + String(giay) : String(giay);
    
  if(gio!=a_gio){
        tft.fillRect(60,35 ,50,40 , TFT_BLACK);
        a_gio = gio;
    }
  if(phut!=a_phut){
        tft.fillRect(130,35 ,50,40 , TFT_BLACK);
        a_phut = phut;
    }

// Clock
  tft.setCursor(60,35);
  tft.setTextSize(4);
  tft.setTextColor(TFT_GREEN);
  //tft.println(String(chuoiGio) + ":" + chuoiPhut + ":" + chuoiGiay);
  tft.println(String(chuoiGio + ":" + chuoiPhut));
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW);

  
  
  for (int i = 0; i < 3; i++) {
    if (hours[i] == gio && minutes[i] == phut) {
    if (giay < 5 && relay4State == 0){
        relay4State = 1;
        lastimetocontrol2 = millis();
        digitalWrite(relay4, HIGH);
        }
    }
  }
   
  if ((relay4State == 1)&&((millis() - lastimetocontrol2) >= (amount1 * 1000))) 
  {
        relay4State = 0;
        digitalWrite(relay4, LOW); // Deactivate relay
  }
  
  Firebase.getInt(fbdo,"feed");
  feed = fbdo.intData();
    if(feed == 1){
      digitalWrite(relay4, HIGH);
    }else if(feed == 0 && relay4State == 0){
      digitalWrite(relay4, LOW);
    }

  Firebase.getInt(fbdo,"Mode");
  mode = fbdo.intData();
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.setCursor(130,90);
  if(mode == 1)
  {
    tft.fillRect(130, 90 , 110, 25 , TFT_BLACK);tft.println(F("Manual"));
    Firebase.getInt(fbdo,"Thietbimot");
    value1 = fbdo.intData();
    if(value1==1){
        digitalWrite(relay1, HIGH);
      }
    else{
        digitalWrite(relay1, LOW);
      }
    
    Firebase.getInt(fbdo,"Thietbihai");
    value2 = fbdo.intData();
    if(value2==1){
        digitalWrite(relay2, HIGH);
      }
    else{
        digitalWrite(relay2, LOW);
      }
    
    Firebase.getInt(fbdo,"Thietbiba");
    value3 = fbdo.intData();
    if (value3==1){
        digitalWrite(relay3, HIGH);
      }
    else{
        digitalWrite(relay3, LOW);
      }

    if((waterlevel>=Nguongnuoc) || (waterlevel == 15)){
        Firebase.setInt( fbdo,"Thietbiba", 0);
        digitalWrite(relay3, LOW);
      }
  }
  else{
    digitalWrite(relay2, HIGH);
    Firebase.setInt( fbdo,"Thietbihai", 1);
    tft.fillRect(130, 90 , 110, 25 , TFT_BLACK);tft.println(F("Auto"));
    //Firebase.getInt(fbdo,"nguongnhiet");
    //Nguongnhiet = fbdo.intData();
    if(tempC>=Nguongnhiet){
        digitalWrite(relay1, HIGH);
        Firebase.setInt( fbdo,"Thietbimot", 1);
      }
    else{
        digitalWrite(relay1, LOW);
        Firebase.setInt( fbdo,"Thietbimot", 0);
      }
    //Firebase.getInt(fbdo,"nguongnuoc");
    //Nguongnuoc = fbdo.intData();
    if(waterlevel<Nguongnuoc){
        digitalWrite(relay3, HIGH);
        Firebase.setInt( fbdo,"Thietbiba", 1);
      }
    else{
        digitalWrite(relay3, LOW);
        Firebase.setInt( fbdo,"Thietbiba", 0);
      }
  }
}


