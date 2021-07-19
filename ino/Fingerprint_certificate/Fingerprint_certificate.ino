const char* host = "https://api.github.com";
const char* fingerpr = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";
            
WiFiClientSecure client;
client.connect(host, httpsPort);

if (client.verify(fingerpr, host)) 
{  
    http.begin(client, host);

    String payload;
    if (http.GET() == HTTP_CODE_OK)    
        payload = http.getString();    
}
else 
{
  Serial.println("certificate doesn't match");
}
