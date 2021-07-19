#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino_JSON.h>


ESP8266WiFiMulti WiFiMulti;
HTTPClient https;
WiFiClientSecure client;

String payload;
int debug = 0;
float json_values[2];

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WebCube-AE17", "7Y33B28Y");
  WiFiMulti.addAP("Bocchio' s phone", "Superme01");

  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);
}

void loop()
{
  if (WiFiMulti.run() == WL_CONNECTED)
  {
    client.setInsecure();
    if (https.begin(client, "https://bocchio.altervista.org/SubDomain/IoT/iot_test.php"))
    {
      int httpCode = https.GET();
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        payload = https.getString();
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
      }
      else
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      https.end();
    }
    else
      Serial.printf("[HTTPS] Unable to connect\n");
  }  
  digitalWrite(D1, LOW);
  delay(10000);
  digitalWrite(D1, HIGH);
}
