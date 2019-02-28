

#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>






/*** ESP8266 WiFi Kütüphane Dosyası ***/
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "smarttrashcan-93d95.firebaseio.com"                         //database api url
#define FIREBASE_AUTH "fc5cbAtPXIhaP64qG8QYj87aRvIvdPhSWAtqJJ95"           //database secret

/*** Kablosuz Ağ Bilgileri ***/
//#define   WLAN_SSID     "Chewie"   // 
//#define   WLAN_PASSWORD "kpiMx2GE"  // 
#define   TRASH_ID "0/id" 
#define   TRASH_NAME "0/name"
#define   TRASH_ISFULL "0/isFull" 
#define   TRASH_CYCLE_COUNT "0/cycleCount"  
#define   TRASH_MAXRANGE "0/maxRange" 
#define   TRASH_DISTANCE "0/distance"  
#define   TRASH_FILLRATE "0/fillRate" 

  
#define   WLAN_SSID     "AndroidAP"   // 
#define   WLAN_PASSWORD "abuzer1453"  // 

/*** Değişken ve Pin Tanımlamaları ***/
int sicaklikSensor= A0;
float sicaklikdegeri;   //Analog değeri dönüştüreceğimiz sıcaklık değeri 
float olculendeger;   //Ölçeceğimiz analog değer
const int TrigPin = D7;//Bu pinlerde takili olcak triger 7'de
const int EchoPin = D8;//Echo 8'de
int mesafe;
int sure;
int cycle_counter;

/*** ESP8266 WiFi Kurulum Fonksiyonu ***/
void WiFi_Setup(){
 delay(10);
 Serial.println(); Serial.print(WLAN_SSID);
 Serial.print("Kablosuz Agina Baglaniyor");
 WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
 // WiFi durum kontrolü
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();Serial.print(WLAN_SSID);
  Serial.println("Kablosuz Agina Baglandi");
  Serial.println("IP adresi: "); 
  Serial.println(WiFi.localIP());
}


void setup() {
  cycle_counter = 0;
  Serial.begin(115200);  // Seri port baud rate ayarı
  WiFi_Setup();          //Kablosuz ağ bağlantı fonksiyonu
 
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin , INPUT);
  
  Firebase.setInt(TRASH_ID,0);
  Firebase.setString(TRASH_NAME,"trashDefault");
  Firebase.setInt(TRASH_CYCLE_COUNT,0);
  Firebase.setInt(TRASH_DISTANCE,0);
  //Firebase.setInt(TRASH_ISFULL,0);
  Firebase.setInt(TRASH_MAXRANGE,distanceCounter());

}

int checkFillRate()
{
  int range=Firebase.getInt(TRASH_MAXRANGE);
  int currDist=Firebase.getInt(TRASH_DISTANCE);
  double x = (range - currDist) / range;
  return  x * 100;
}


int distanceCounter()
{
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(100000);
  digitalWrite(TrigPin, LOW);
  sure = pulseIn(EchoPin , HIGH); 
  mesafe = (sure / 2 )/29.1; 
  Serial.print(mesafe);
  Serial.println(" CM");

  return mesafe;
}

void loop() {
  Firebase.set(TRASH_DISTANCE,distanceCounter());

  //int fillrate=checkFillRate();
  //if(fillrate >= 85)
    //Firebase.setInt(TRASH_ISFULL,1);

  //Firebase.setInt(TRASH_FILLRATE,fillrate);
  int dist = Firebase.getInt(TRASH_DISTANCE);
  Serial.print("Distance : ");
  Serial.println(dist);
  
  //Serial.print("Fill Rate : ");
  //Serial.println(checkFillRate());

  cycle_counter = cycle_counter + 1;

  Firebase.setInt(TRASH_CYCLE_COUNT,cycle_counter);
  delay(5000);

}
