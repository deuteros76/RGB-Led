
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>

#define RED_PIN 4  //(D1)
#define GREEN_PIN 14 //(D2) 
#define BLUE_PIN 12  //(D3)

const char WiFiAPPSK[] = "rgb12345";       

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);

            if(payload[0] == '#') {
                // we get RGB data

                // decode rgb data
                uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);

                analogWrite(RED_PIN,    ((rgb >> 16) & 0xFF) <<2); //PWM on a ESP8266 is from 0 to 1024, so the last <<2 (we are multiplying by 4)
                analogWrite(GREEN_PIN,  ((rgb >> 8) & 0xFF) <<2);
                analogWrite(BLUE_PIN,   ((rgb >> 0) & 0xFF) <<2);
            }
            break;
    }
}           

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP8266-RGB", WiFiAPPSK);
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Setting up... ");

  //Engine channels  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 1023);
  analogWrite(GREEN_PIN, 0);
  analogWrite(BLUE_PIN, 0);
  
  delay(1000);//wait for a second

  setupWiFi();

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  
  if(MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  // handle index
  server.on("/", []() {
    // send index.html
    server.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendRGB() {  var r = parseInt(document.getElementById('r').value).toString(16);  var g = parseInt(document.getElementById('g').value).toString(16);  var b = parseInt(document.getElementById('b').value).toString(16);  if(r.length < 2) { r = '0' + r; }   if(g.length < 2) { g = '0' + g; }   if(b.length < 2) { b = '0' + b; }   var rgb = '#'+r+g+b;    console.log('RGB: ' + rgb); connection.send(rgb); }</script></head><body>LED Control:<br/><br/>R: <input id=\"r\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"sendRGB();\" /><br/>G: <input id=\"g\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"sendRGB();\" /><br/>B: <input id=\"b\" type=\"range\" min=\"0\" max=\"255\" step=\"1\" onchange=\"sendRGB();\" /><br/></body></html>");
  });
  
  server.begin();
  
  // Add service to MDNS
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);

  analogWrite(RED_PIN, 0);
  analogWrite(GREEN_PIN, 1023);
  analogWrite(BLUE_PIN, 0);
}

void loop()
{  
  webSocket.loop();
  server.handleClient();  
}
