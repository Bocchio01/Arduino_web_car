#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

const char* ssid = "WebCube-AE17"; //Enter SSID
const char* password = "7Y33B28Y"; //Enter Password
const char* websockets_server = "wss://curious-wealthy-pigment.glitch.me/"; //server adress and port

using namespace websockets;

WebsocketsClient client;

float json_values[2];
String payload;

void onMessageCallback(WebsocketsMessage message) {
    //Serial.print("Got Message: ");
    //Serial.println(message.data());
    payload = message.data();
    //Serial.println(payload);
    
        JSONVar myObject = JSON.parse(payload);
        if (JSON.typeof(myObject) == "undefined")
        {
          Serial.println("Parsing input failed!");
          return;
        }
      
        JSONVar keys = myObject.keys();
        for (int i = 0; i < keys.length(); i++)
        {
          JSONVar value = myObject[keys[i]];
          Serial.print(keys[i]);
          Serial.print(" = ");
          json_values[i] = double(value);
          Serial.println(json_values[i]);
        }
    analogWrite(D1, json_values[0]);
    analogWrite(D4, json_values[1]);
    
}

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed, tring to reconnect...");
         bool connected = client.connect(websockets_server);
    if(connected) {
        client.send("[ARDUINO]");
    } else {
        Serial.println("Not Connected!");
    }
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void setup() {
    Serial.begin(115200);
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Setup Callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    
    // Connect to server
 bool connected = client.connect(websockets_server);
    if(connected) {
        client.send("[ARDUINO]");
    } else {
        Serial.println("Not Connected!");
    }
    
    // Send a ping
    client.ping();

    pinMode(D1, OUTPUT);
    digitalWrite(D1, LOW);
        pinMode(D4, OUTPUT);
    digitalWrite(D4, LOW);
}

void loop() {
    client.poll();
}
