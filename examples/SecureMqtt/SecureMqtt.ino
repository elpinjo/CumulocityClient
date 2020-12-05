#include <CumulocityClient.h>
#ifdef ESP8266
    #include <ESP8266WiFi.h>
#else //ESP32
    #include <WiFiClientSecure.h>
#endif

const char *ssid = "........";
const char *wifiPassword = "........";
char *host = "xxx.cumulocity.com";
char *username = "........";    // fixed credentials can be registered in the Administration section
char *c8yPassword = "........"; // create a user in usermanagement with the "device"role and fill the credentials here
char *tenant = "........";      //tenant ID can be found by clicking on your name in the top right corner of Cumulocity
char *clientId = ".........";   //Should be a unique identifier for this device, e.g. IMEI, MAC address or SerialNumber
const char *root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    ".....CERT CONTENT............\n"
    "-----END CERTIFICATE-----\n";

WiFiClientSecure wifiClient;
CumulocityClient c8yClient(wifiClient, clientId);

void setup()
{
    Serial.begin(115200);

    wifiClient.setCACert(root_ca);
    WiFi.begin(ssid, wifiPassword);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("connected to wifi");

    c8yClient.connect(host, 8883, tenant, username, c8yPassword);
    c8yClient.registerDevice(clientId, "c8y_esp32");
}

void loop()
{
    delay(3000);
    c8yClient.loop();

    int8_t rssi = WiFi.RSSI();
    char rssiStr[10];
    sprintf(rssiStr, "%d", rssi);
    c8yClient.createMeasurement("SignalStrength", "T", rssiStr, "*db");
}