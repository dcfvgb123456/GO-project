#include <SoftwareSerial.h>       //Software Serial library
SoftwareSerial espSerial(3,2);   //Pin 2 and 3 act as RX and TX. Connect them to TX and RX of ESP8266      
#define DEBUG true
String mySSID = "EECL_510-2";       // WiFi SSID
String myPWD = "0952256386"; // WiFi Password
String myAPI = "KJ93BRCFEGKNOTLD";   // API Key
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD1 = "field1"; 
String myFIELD2 = "field2"; 
int sendVal1,sendVal2,trig;
char A=0,B=0,C=0;


void setup()
{
  Serial.begin(9600);
  espSerial.begin(9600);
  
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  /*while(!esp.find("OK")) 
  {          
      //Wait for connection
  }*/
  delay(1000);
  
}

  void loop()
  {
    /* Here, I'm using the function random(range) to send a random value to the 
     ThingSpeak API. You can change this value to any sensor data
     so that the API will show the sensor data  
    */
    A = 13; B = 12;
    pinMode(A,OUTPUT);
    digitalWrite(A,LOW);
    delayMicroseconds(2);
    digitalWrite(A,HIGH);
    delayMicroseconds(10);
    pinMode(B,INPUT);
    C = pulseIn(B,HIGH,30000)/58.0;
    sendVal1 =((((analogRead(A0)*50)/10)-500)/10); // Send a random number between 1 and 1000
    sendVal2 =(C);
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD1 +"="+String(sendVal1)+"&"+ myFIELD2 +"="+String(sendVal2);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    if( espSerial.find( "Error" ) )
    {
        Serial.print( "RECEIVED: Error\nExit1" );
        
        return;
     
    }
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
   // Serial.println(sendVal);
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(10000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
 Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
    //Serial.print(response);
  }
  return response;
  }
  
