#include <CumulocityClient.h>
#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include "FS.h"
#include "SPIFFS.h"

/* You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin */
#define FORMAT_SPIFFS_IF_FAILED true
#define CREDENTIALS_FILE "/credentials.txt"

const char* ssid = "....";
const char* wifiPassword = ".....";
char* host = "xxx.cumulocity.com";
char* username = "...."; //bootstrap credentials can be requested through support
char* c8yPassword = "....";
char* tenant = "....";
char clientId[20];
bool storedCredentials = false;

WiFiClient wifiClient;
CumulocityClient c8yClient(wifiClient, clientId);

//Serial Number taken from the ESP32, not tested on ESP8266
void getSerialNumber() {

  uint64_t chipid = ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid >> 32);

  snprintf(clientId, 19, "ESP32-%04X%08X", chip, (uint32_t)chipid);

  Serial.printf("Serial Number is: %s\n", clientId);
}

//connect to a WiFi access point
void connectWifi() {
  WiFi.begin(ssid, wifiPassword);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println("connected to wifi");
}

//read device credentials from the SPIFFS filesystem
void readCredentials(fs::FS &fs, const char *path) {
  Serial.printf("Reading file: %s\r\n", path);

  char buffer[4096];
  
  File file = fs.open(path, FILE_READ);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to read the file");
    return;
  }
  
  int length = 0;
  while(file.available()){
    char c = file.read();
    buffer[length++] = c;
  }
  buffer[length] = '\0';
  
  tenant = strtok(buffer, "\n");
  Serial.printf("Retrieved tenant: %s\n", tenant);

  username = strtok(NULL, "\n");
  Serial.printf("Retrieved user: %s\n", username);

  c8yPassword = strtok(NULL, "\n");

  if (tenant != NULL && username != NULL && c8yPassword != NULL) {
    Serial.printf("found credentials for %s, %s\n", tenant, username);
    storedCredentials = true;
  }

  file.close();
}

//write data to a file
void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

//add data to a file
void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

// store retrieved credentials to file
void storeCredentials() {
  
  Credentials myCredentials = c8yClient.getCredentials();

  Serial.printf("Writing credentials %s/%s to file", myCredentials.tenant, myCredentials.username);
  
  writeFile(SPIFFS, CREDENTIALS_FILE, myCredentials.tenant);
  appendFile(SPIFFS, CREDENTIALS_FILE, "\n");
  appendFile(SPIFFS, CREDENTIALS_FILE, myCredentials.username);
  appendFile(SPIFFS, CREDENTIALS_FILE, "\n");
  appendFile(SPIFFS, CREDENTIALS_FILE, myCredentials.password);
  appendFile(SPIFFS, CREDENTIALS_FILE, "\n");

  Serial.println("Credentials stored");
}

//make a connection to cumulocity
void connectC8Y() {
  
  c8yClient.connect(host, tenant, username, c8yPassword);
  
  Serial.println("Retrieving device credentials");

  if (!storedCredentials) {
    c8yClient.retrieveDeviceCredentials();
    while (!c8yClient.checkCredentialsReceived()) {
      Serial.print("#");
      delay(1000);
    }
  
    Serial.println("Reconnecting to Cumulocity");
    
    c8yClient.disconnect();
    c8yClient.reconnect();
  }
}

//============================================================================
// Setup and Loop methods
//============================================================================

void setup() {

  Serial.begin(115200);
  
  connectWifi();

  getSerialNumber();

  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
      Serial.println("SPIFFS Mount Failed");
      return;
  }

  readCredentials(SPIFFS, CREDENTIALS_FILE);
  
  connectC8Y();

  if (!storedCredentials) {
    storeCredentials();
  }

  c8yClient.registerDevice(clientId, "c8y_esp32");
    
}

void loop() {
  
  delay(1000);
  c8yClient.loop();
  
}