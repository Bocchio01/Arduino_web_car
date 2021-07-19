#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
#define SERVER_NAME "https://bocchio.altervista.org/Home.html" // put the host name here like  . this is an example that works :)
#define WIFI_SSID "WebCube-AE17"
#define WIFI_PASS "7Y33B28Y"

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
void setup() {

    pinMode(D1, OUTPUT);
    pinMode(D4, OUTPUT);
    USE_SERIAL.begin(9600); // Init Serial comms

    // Clear Serial buffer
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println("Clearing Serial buffer...");
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.flush();
        delay(1000);
    }

    // Add access point to WiFimulti
    WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        digitalWrite(D4, HIGH);

        // configure and initiate connection with target server and url
        HTTPClient http;
        USE_SERIAL.print("[HTTP] starting...\n");
        http.begin(client, SERVER_NAME);

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error. Success http code is 200
        if(httpCode > 0) {
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // receive response from the Server
            if(httpCode == HTTP_CODE_OK) {
                digitalWrite(D1, LOW);
                // On successful connection
                USE_SERIAL.print("[HTTP] Received HTML...\n");
                String payload = http.getString();
                USE_SERIAL.println(payload);
                USE_SERIAL.flush();
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        delay(1000);
        digitalWrite(D1, HIGH);
    }
    // 10 second delay, since we don't want to overload the target Server
    else digitalWrite(D4, LOW);
    delay(10000);
}
