#include <DFRobot_SHT20.h>
#define MQ2pin 0
#include <SPI.h>
#include <Wire.h>
#include <WiFi101.h>
#include <ArduinoJson.h>
float sensorValue;  //variable to store sensor value
float temp;
float humd;
String rawbufjson;
DFRobot_SHT20 sht20;
int ledpin = 6;


//DynamicJsonDocument doc(1024);

//doc["gas"] = 999.99;
//doc["temp"] = 100.99;
//doc["humd"] = 100.09;


#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

void setup() {
  digitalWrite(ledpin, HIGH);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(3000);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWiFiStatus();
  Serial.println("SHT20 Example!");
  sht20.initSHT20();                         // Init SHT20 Sensor
  delay(100);
  sht20.checkSHT20();                        // Check SHT20 Sensor
  digitalWrite(ledpin, LOW);
}


void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) {
    digitalWrite(ledpin, HIGH);
    Serial.println("new client");
    // an http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          //doc = [];
          StaticJsonDocument<500> doc;
          //doc[""] = "";
          doc["temp"] = sht20.readTemperature();
          doc["humd"] = sht20.readHumidity();
          doc["ppm"] = analogRead(MQ2pin); 
          serializeJson(doc, rawbufjson);
          client.println("HTTP/1.1 200 OK");
          client.println("Access-Control-Allow-Origin: *");
          client.println("Content-Type: application/json");
          client.print("Content-Length: ");
          client.println(measureJson(doc));
          //client.print()
          //client.println("Connection: close");  // the connection will be closed after completion of the response
          //client.println("Refresh: 2");  // refresh the page automatically every 5 sec
          client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
          client.println(rawbufjson);
          rawbufjson = "";
          //client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);

    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    digitalWrite(ledpin, LOW);

  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
