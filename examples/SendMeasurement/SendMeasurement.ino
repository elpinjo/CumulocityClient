#include <CumulocityClient.h>
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif

const char* ssid = "........";
const char* wifiPassword = "........";
char* host = "xxx.cumulocity.com";
char* username = "........";  // fixed credentials can be registered in the Administration section
char* c8yPassword = "........"; // create a user in usermanagement with the "device"role and fill the credentials here
char* tenant = "........"; //tenant ID can be found by clicking on your name in the top right corner of Cumulocity
char* clientId = "........."; //Should be a unique identifier for this device, e.g. IMEI, MAC address or SerialNumber
//uint64_t chipid = ESP.getEfuseMac();

WiFiClient wifiClient;
CumulocityClient c8yClient(wifiClient, clientId);

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected to wifi");

  c8yClient.connect(host, tenant, username, c8yPassword);

  c8yClient.registerDevice(clientId, "c8y_esp32");

}

void loop() {

  delay(1000);
  c8yClient.loop();
  c8yClient.createMeasurement("Temperature", "T", "20.5", "*C");

}
