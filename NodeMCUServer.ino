/*
 * This is to test NodeMCU as a Webserver
 * sending request via url will respond with
 * 136,5,55,34,152
 * 202,246,56,59,63
 * 136,4,246,44,86
 * 136,4,247,44,87
 * 136,4,248,44,88
 * will give you a text with the  number as a response
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "NETWORK_HERE";
const char* password = "NETWORK_KEY_HERE";

ESP8266WebServer server(80);

//Need to design better Page BUT THIS IS FOR TEST ONLY
String page = "<h1>NodeMCU Web Server</h1>";

void handleRoot() {
  server.send(200, "text/html", page);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  // put your setup code here, to run once:
  page ="NodeMCU Web Server";
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to Wifi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConnected to: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/136,5,55,34,152", [](){
    server.send(200, "text/plain", "Venezia: 136,5,55,34,152");
  });

  server.on("/202,246,56,59,63", [](){
    server.send(200, "text/plain", "Blank card: 202,246,56,59,63");
  });

  server.on("/136,4,246,44,86", [](){
    server.send(200, "text/plain", "NXP 2: 136,4,246,44,86");
  });

  server.on("/136,4,247,44,87", [](){
    server.send(200, "text/plain", "NXP 3: 136,4,247,44,87");
  });

  server.on("/136,4,248,44,88", [](){
    server.send(200, "text/plain", "NXP 4: 136,4,248,44,88");
  }); 

  server.onNotFound(handleNotFound);  

  server.begin();
  Serial.println("\nWeb Server started!");
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
