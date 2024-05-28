#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

void pressButton1();
void pressButton2();
void pressButton3();
void mode2();
void mode3();
bool detectMove();
void takePhotos(int time);
void handleButtonInterrupt();
void takeOnePhoto();

#define button1 00
#define button2 01
#define button3 10
#define yellowLED 02
#define redLED 12
#define greenLED 03
#define blueLED 11
#define ssrCH1Focus 05
#define ssrCH2Shutter 04

const char *ssid = "esp32";
const char *password = "photo_trap_123";
const int udpPort = 1234;

int ssrON, ssrOFF;
const int debounceTime = 30;
int seriesTime = 3000;
byte button1LogicalState = 1;
byte button1LastLogicalState = 1;
byte button2LogicalState = 1;
byte button2LastLogicalState = 1;
byte button3LogicalState = 1;
byte button3LastLogicalState = 1;
int pirValue, dopplerValue;

bool tempB2;
bool tempB3;
volatile bool m2 = false;
volatile bool m3 = false;

int before2;
int after2;
int before3;
int after3;

WiFiUDP udp;
bool take_one = 0;

void IRAM_ATTR isr() {
  if(m2==1 || m3==1)
  {
    m2=0;
    m3=0;
    digitalWrite(greenLED, LOW);
    digitalWrite(blueLED, LOW);
  }
  else
  {
    take_one = 1;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  pinMode(ssrCH1Focus, OUTPUT);
  pinMode(ssrCH2Shutter, OUTPUT);

// Settings for Low trigger relay
  ssrON = LOW;
  ssrOFF = HIGH;
  digitalWrite(ssrCH1Focus, ssrOFF);
  digitalWrite(ssrCH2Shutter, ssrOFF);

  attachInterrupt(button1, isr, RISING);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Brak połączenia z WiFi...");
  }
  Serial.println("Połączono z WiFi");

for (int i = 0; i < 3; i++) {
    digitalWrite(redLED, HIGH);
    delay(500);
    digitalWrite(redLED, LOW);
    delay(300);
  }

  udp.begin(udpPort);
}

void loop() {
  takeOnePhoto();
  detectMove();
  mode2();
  mode3();
  pressButton2();
  pressButton3();
}

void takeOnePhoto()
{
 if (take_one == 1)
 {
  digitalWrite(redLED, LOW);
  takePhotos(100);
  take_one = 0;
 }
}

void pressButton1()
{
  button1LogicalState = digitalRead(button1);
  delay(debounceTime);

  if (button1LastLogicalState != button1LogicalState)
  {
    if (button1LogicalState == 1)
    {
         digitalWrite(redLED, LOW);
      takePhotos(100);
    }
    button1LastLogicalState = button1LogicalState;
  }
}

void pressButton2()
  {
  if(m2==1 && digitalRead(button2) == HIGH)
  {
  button2LogicalState = digitalRead(button2);

  if (button2LastLogicalState != button2LogicalState) {
    if (button2LogicalState == 1) {
      if (m2 == 1) {
        digitalWrite(greenLED, LOW);
        m2 = 0;
      }
    }
    button2LastLogicalState = button2LogicalState;
  }
  }
  else
  {
  button2LogicalState = digitalRead(button2);

  if (button2LastLogicalState != button2LogicalState) {
    if (button2LogicalState == 1) {
      if (m2 == 0) {
        digitalWrite(greenLED, HIGH);
        m2 = 1;
      }
    }
    button2LastLogicalState = button2LogicalState;
  }
  }
}

void pressButton3()
  {
  if(m3==1 && digitalRead(button3) == HIGH)
  {
  button3LogicalState = digitalRead(button3);

  if (button3LastLogicalState != button3LogicalState) {
    if (button3LogicalState == 1) {
      if (m3 == 1) {
        digitalWrite(blueLED, LOW);
        m3 = 0;
      }
    }
    button3LastLogicalState = button3LogicalState;
  //}
  }
  }
  else
  {
  button3LogicalState = digitalRead(button3);

  if (button3LastLogicalState != button3LogicalState) {
    if (button3LogicalState == 1) {
      if (m3 == 0) {
        digitalWrite(blueLED, HIGH);
        m3 = 1;
      }
    }
    button3LastLogicalState = button3LogicalState;
  }
  }
}

void mode2() {
  detectMove();
if (m2 && detectMove()) {
  before2 = digitalRead(button2);
    takePhotos(3000);
    delay(3000);
  }
}

void mode3() {
  detectMove();
  if (m3 && detectMove()) {
    before3 = digitalRead(button3);
    takePhotos(10000);
    delay(3000);
  }
}

bool detectMove()
{
    if (WiFi.status() != WL_CONNECTED){
    pirValue = 0;
    dopplerValue = 0;
  }
  
  int packetSize = udp.parsePacket();
  if (packetSize >= sizeof(int) * 2) {

    udp.read((uint8_t*)&pirValue, sizeof(pirValue));
    udp.read((uint8_t*)&dopplerValue, sizeof(dopplerValue));

    Serial.print("Odebrano dane - PIR: ");
    Serial.print(pirValue);
    Serial.print(", Doppler: ");
    Serial.println(dopplerValue);
  }


  if (pirValue==1 && dopplerValue==1)
  {
    digitalWrite(yellowLED, HIGH);
    return true;
  }
  else if (pirValue==0)
  {
    digitalWrite(yellowLED, LOW);
    return false;
  }
}

void takePhotos(int time)
{
  digitalWrite(ssrCH1Focus, ssrON);
  digitalWrite(ssrCH2Shutter, ssrOFF);
  delay(2000);
    digitalWrite(redLED, HIGH);
  digitalWrite(ssrCH2Shutter, ssrON);
  delay(time);
  digitalWrite(ssrCH1Focus, ssrOFF);
  digitalWrite(ssrCH2Shutter, ssrOFF);
    digitalWrite(redLED, LOW);
}