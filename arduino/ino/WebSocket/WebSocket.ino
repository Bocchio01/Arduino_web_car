#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>

#include <Servo.h>

using namespace websockets;
WebsocketsClient client;
Servo steering_servo;

const char *ssid = "WebCube-AE17";
const char *password = "7Y33B28Y";
// const char *websockets_server = "wss://curious-wealthy-pigment.glitch.me/";
// const char *websockets_server = "wss://scientific-proximal-basement.glitch.me/";
const char *websockets_server = "ws://192.168.1.161:5000";

int json_values[2];
char buffer[40];

void onMessageCallback(WebsocketsMessage message)
{
  JSONVar myObject = JSON.parse(message.data());
  JSONVar primary_keys = myObject.keys();
  for (int j = 0; j < primary_keys.length(); j++)
  {
    JSONVar value_obj = myObject[primary_keys[j]];
    JSONVar sub_keys = value_obj.keys();
    for (int i = 0; i < sub_keys.length(); i++)
    {
      JSONVar value = value_obj[sub_keys[i]];
      json_values[i] = double(value);

      Serial.print(sub_keys[i]);
      sprintf(buffer, " = %d; ", json_values[i]);
      Serial.print(buffer);
    }
    Serial.println();

    if ((int)json_values[0] < 0)
    {
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
    }
    else
    {
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
    }

    analogWrite(D3, (int)(255 / 20 * abs(json_values[0])));
    steering_servo.write((int)(65 + 13 * json_values[1]));
  }
}

void onEventsCallback(WebsocketsEvent event, String data)
{
  switch (event)
  {
  case WebsocketsEvent::ConnectionOpened:
    client.send("{\"type\":\"HandShake\",\"value\":\"Arduino\",\"option\":\"only_data\"}");
    Serial.println("Connection open: HandShake");
    break;
  case WebsocketsEvent::ConnectionClosed:
    Serial.println("Connection close: restarting shield");
    ESP.restart();
    break;
  }
}

void setup()
{
  Serial.begin(115200);

  // Start connection to WiFI
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  for (int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++)
  {
    Serial.print(".");
    delay(1000);
  }

  WiFi.status() == WL_CONNECTED ? Serial.println("Connected to WiFi") : Serial.println("Not connected to WiFi");

  // Define event function for WebSocket and check server connection
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  client.connect(websockets_server) ? Serial.println("Connected to Server!") : Serial.println("Not Connected to Server!");

  // Setup pins for steering and drive motor
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, HIGH);
  steering_servo.attach(D7);
}

void loop()
{
  client.poll();
}
