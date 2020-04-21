/*
  Esp8266 Websockets Client

	This sketch:
        1. Connects to a WiFi network
        2. Connects to a Websockets server
        3. Sends the websockets server a message ("Hello Server")
        4. Prints all incoming messages while the connection is open

  Hardware:
  For this sketch you only need an ESP8266 board.
  Created 15/02/2019
  By Gil Maimon
  ttps://github.com/gilmaimon/ArduinoWebsockets

*/


/*Format fot data transfer
  value 0 indicative of ON/ Ok

  value 1 indicative of OFF/ fault

  vLSB
  |_00_|_01_|_02_|_03_|_04_|_05_|_06_|_07_|_08_|_09_|_10_|_11_|_12_|_13_|_14_|_15_|

  |__EDC number 0-16__|stat|norm|altr|trip|_**_|_**_|_**_|_**_|_**_|_**_|_**_|_**_|

  |_16_|_17_|_18_|_19_|_20_|_21_|_22_|_23_|_24_|_25_|_26_|_27_|_28_|_29_|_30_|_31_|

  |_**_|_**_|_**_|_**_|_**_|_**_|ad01|ad02|ad03|ad04|ad05|ad06|ad07|ad08|ad09|ad10|


  EDC number   =bit 0-3
  EDC status   =bit 4
  normal supply=bit 5
  altern supply=bit 6
  recttifer trp=bit 7
  future use   =bit 8-21
  10 bit adc   =bit 22-31

*/


#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
//#define EDCIP 97///////////////////////////////////////////////////
#define EDC 15
#define normalStatusPin D5
#define alternateStatusPin D6
#define tripStatusPin D7
#define wifiStatus D1
#define applicationStatus D2


//#define IP 192, 168, 1, EDCIP
#define GW 192, 168, 1, 1
#define SN 255, 255, 255, 0
//#define EDC EDCIP-90/////////////////////////////////////////////////////////////
//String edc=String(EDC);////////////////////////////////////////////////////////

//String statusOk="EDC#"+edc+"#YES";
//String statusNOk="EDC#"+edc+"#NO";

String statusMessage;///////////////////////////////////////////////////////
const char* ssid = "sherinkapotein"; //Enter SSID
const char* password = "1nt3rl@k3n"; //Enter Password
const char* websockets_server_host = "192.168.1.163"; //Enter server adress
const uint16_t websockets_server_port = 40510; // Enter server port
bool connected;
using namespace websockets;

uint8_t edc = EDC;
bool edcStatus;
bool normalStatus ;
bool alternateStatus;
bool tripStatus;
int adcRead=0;
int message=0;

WebsocketsClient client;


void setup()
{
  // pinMode(edcStatus, INPUT_PULLUP);
  pinMode(normalStatus, INPUT_PULLUP);
  pinMode(alternateStatus, INPUT_PULLUP);
  pinMode(tripStatus, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(115200);
  // Connect to wifi
  //Static IP address configuration

  /* IPAddress staticIP(IP); //ESP static ip
    IPAddress gateway(GW);   //IP Address of your WiFi Router (Gateway)
    IPAddress subnet(SN);  //Subnet mask

    WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
    WiFi.config(staticIP, subnet, gateway);
  */
  WiFi.begin(ssid, password);

  // Wait some time to connect to wifi
  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
  {
    Serial.print(".");
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH);

  }

  // Check if connected to wifi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("No Wifi!");
    return;
  }
  Serial.println(WiFi.localIP());

  Serial.println("Connected to Wifi, Connecting to server.");
  // try to connect to Websockets server
  digitalWrite(LED_BUILTIN, LOW);
  connected = client.connect(websockets_server_host, websockets_server_port, "/");
  digitalWrite(LED_BUILTIN, HIGH);
  if (connected) {
    Serial.println("Connecetd!");
    client.send("SENDING FROM WEMOS CLIENT");
  } else {
    Serial.println("Not Connected!");
    digitalWrite(LED_BUILTIN, LOW);
  }

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());

    if ((message.data()) == "yes")
    { Serial.println("RECIEVED DATA FROM SERVER");
      // ESP.restart();
    }
  });
}

void loop() {

  if (client.available())
  {
    client.poll();
    /// edcState = digitalRead(edcStatus);
    normalStatus    = digitalRead(normalStatusPin);
    alternateStatus = digitalRead(alternateStatusPin);
    tripStatus      = digitalRead(tripStatusPin);
    adcRead             = 1023;
    edcStatus= !(normalStatus ^ alternateStatus)  |  tripStatus;

    message=0| (adcRead << 22);
    message |= (0x0f & edc) | (edcStatus<< 4)| (normalStatus << 5)
            |  (alternateStatus<< 6) | (tripStatus<< 7);

    Serial.print("ADC= ");Serial.println(adcRead, BIN);
    Serial.print("Final= ");Serial.println(message,BIN);
    Serial.print("edc= ");Serial.println(edc,BIN);
    Serial.println(edcStatus,BIN);
    Serial.println(normalStatus,BIN);
    Serial.println(alternateStatus,BIN);
    Serial.println(tripStatus,BIN);
    //        Serial.println(normalState);
    //        Serial.println(alternateState);
    //        Serial.println(tripState);

    /*      // statusMessage="EDC#"+edc+"#"+String(edcState)+"#";
           statusMessage+=String(normalState)+"#";
           statusMessage+=String(alternateState)+"#";
           statusMessage+=String(tripState);
           Serial.println(statusMessage);
           if(!client.send(statusMessage))
           {Serial.println("UNABLE TO SEND WEBSOCKET MESSAGE");};

       }

    */
    delay(500);


    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.print("WIFI DISCONNECTION DETECTED!!!");
      // Connect to wifi

      //Static IP address configuration
      /*    IPAddress staticIP(IP); //ESP static ip
          IPAddress gateway(GW);   //IP Address of your WiFi Router (Gateway)
          IPAddress subnet(SN);  //Subnet mask
          WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
          WiFi.config(staticIP, subnet, gateway);
      */

      WiFi.begin(ssid, password);

      // Wait some time to connect to wifi
      for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
      {
        Serial.print(".");
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);                      // Wait for a second
        digitalWrite(LED_BUILTIN, HIGH);

      }

      // Check if connected to wifi
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
      }
      Serial.println(WiFi.localIP());


      Serial.println("Connected to Wifi, Connecting to server.");
      // try to connect to Websockets server

      connected = client.connect(websockets_server_host, websockets_server_port, "/");
      if (connected) {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Connecetd!");
        // client.send("SENDING FROM WEMOS CLIENT");
      }
      else
      {
        Serial.println("Not Connected!");
        digitalWrite(LED_BUILTIN, HIGH);
      }

      // run callback when messages are received
      client.onMessage([&](WebsocketsMessage message) {
        Serial.print("Got Message: ");
        Serial.println(message.data());
      });




    }

    if (!client.available())
    {
      Serial.println("WEBSOCKET FAILED!!!");
      connected = client.connect(websockets_server_host, websockets_server_port, "/");
      if (connected)
      { digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Connecetd!");
        // client.send("SENDING FROM WEMOS CLIENT");
      }
      else
      {
        Serial.println("Not Connected!");
        delay(500);
        digitalWrite(LED_BUILTIN, LOW);

      }

    }
  }
}
