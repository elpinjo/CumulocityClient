#include <CumulocityClient.h>
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif

#define LED_BUILTIN 19

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

int state = LOW;

//connect to a WiFi access point
void connectWiFi() {
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (tries++ > 10) {
      WiFi.begin(ssid, wifiPassword);
      tries = 0;
    }

  }
  Serial.println("connected to wifi");
}

int switchLed(char* templateCode, char* payload) {

  Serial.printf("switchLed(template: %s, payload: %s)\n", templateCode, payload);

  if (strcmp("CLOSED", payload)==0) {
    state = 1;
  } else {
    state = 0;
  }

  digitalWrite(LED_BUILTIN, state);
  
  return 0;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);

  connectWiFi();

  c8yClient.connect(host, tenant, username, c8yPassword);

  c8yClient.registerDevice(clientId, "c8y_esp32");

  c8yClient.setCallback(switchLed);
  
  c8yClient.setSupportedOperations("c8y_Relay,c8y_Command,c8y_Restart");

  c8yClient.getPendingOperations();
}

void loop() {
  c8yClient.loop();
}