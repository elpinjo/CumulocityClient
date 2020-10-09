#include <CumulocityClient.h>
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif

const char* ssid = "........";
const char* wifiPassword = "........";
const char* host = "xxx.cumulocity.com";
char* username = "........";
char* c8yPassword = "........";
char* tenant = "........";
const char* clientId = "........."; //Should be a unique identifier for this device, e.g. IMEI, MAC address or SerialNumber
//uint64_t chipid = ESP.getEfuseMac();

WiFiClient wifiClient;
PubSubClient pubSubClient(host, 1883, wifiClient);
CumulocityClient c8yClient(pubSubClient, tenant, username, c8yPassword, clientId);

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  c8yClient.connect();

  Serial.println("Retrieving device credentials");

  c8yClient.retrieveDeviceCredentials();
  while (!c8yClient.checkCredentialsReceived()) {
    Serial.print("#");
    delay(1000);
  }

  c8yClient.disconnect();
  c8yClient.connect();

  c8yClient.registerDevice("ESP32 - Misja", "c8y_esp32");

}

void loop() {

  delay(1000);
  c8yClient.loop();

}
