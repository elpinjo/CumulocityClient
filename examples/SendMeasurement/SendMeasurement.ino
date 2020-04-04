#include <CumulocityClient.h>
#include <WiFi.h>

const char* ssid = "........";
const char* wifiPassword = "........";
const char* host = "xxx.cumulocity.com";
char* username = "........";
char* c8yPassword = "........";
char* tenant = "........";
const char* clientId = "........."; //Should be a unique identifier for this device, e.g. IMEI, MAC address or SerialNumber
//uint64_t chipid = ESP.getEfuseMac();

WiFiClient wifiClient;
CumulocityClient c8yClient(wifiClient, host, tenant, username, c8yPassword, clientId);

void setup() {

  Serial.begin(115200);
  
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  c8yClient.connect();
  
  Serial.println("Retrieving device credentials");
  while (!c8yClient.checkCredentialsReceived()) {
    Serial.print("#");
  }
  c8yClient.disconnect();
  c8yClient.connect();
  
  c8yClient.registerDevice("ESP32 - Misja", "c8y_esp32");

    
}

void loop() {
  
}