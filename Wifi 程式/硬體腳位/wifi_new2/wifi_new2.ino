#include <SoftwareSerial.h>       //Software Serial library   
#define DEBUG true
String mySSID = "EECL_510-2";       // WiFi SSID
String myPWD = "0952256386"; // WiFi Password
String myAPI = "RIJZQTNUE1RLYEE6";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD1 = "field1";
String myFIELD2 = "field2";
int sendVal1;
int sendVal2;
int trig;
char A = 0;
char B = 0;
char C = 0;

void setup()
{
  Serial.begin(9600);
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\"" + mySSID + "\",\"" + myPWD + "\"", 1000, DEBUG); //Connect to WiFi network
  delay(1000);
}

void loop()
{
  A = 13; B = 12;
  pinMode(A, OUTPUT);
  digitalWrite(A, LOW);
  delayMicroseconds(2);
  digitalWrite(A, HIGH);
  delayMicroseconds(10);
  pinMode(B, INPUT);
  C = pulseIn(B, HIGH, 30000) / 58.0;

  sendVal1 = ((((analogRead(A0) * 50) / 10) - 500) / 10); // Send a random number between 1 and 1000
  sendVal2 = (C);
  //if(C<=1){return 400;} else return C;
  String sendData = "GET /update?api_key=" + myAPI + "&" + myFIELD1 + "=" + String(sendVal1) + "&" +  myFIELD2 + "=" + String(sendVal2);
  espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
  espData("AT+CIPSTART=0,\"TCP\",\"" + myHOST + "\"," + myPORT, 1000, DEBUG);
  espData("AT+CIPSEND=0," + String(sendData.length() + 4), 1000, DEBUG);
  Serial.find(">");
  Serial.println(sendData);
  espData("AT+CIPCLOSE=0", 1000, DEBUG);
  delay(5000);
}

String espData(String command, const int timeout, boolean debug)
{
  String response = "";
  Serial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial.available())
    {
      char c = Serial.read();
      response += c;
    }
  }
  return response;
}
