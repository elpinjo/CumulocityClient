/*
 * Cumulocity.h - A client library for Arduino leveraging MQTT to connect to Cumulocity IoT platform.
 * The library allows to send measurement to the platform and to contol the device through operations.
 * This library relies on the PubSubClient library by Nick O'Leary - @knolleary for using MQTT
 * Created by M.J. Heuveling, Software AG, 2 May 2020
 * misja.heuveling@softwareag.com
 */

#ifndef CUMULOCITY_CLIENT_H
#define CUMULOCITY_CLIENT_H

#include "Arduino.h"
#include <PubSubClient.h>

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define C8Y_CALLBACK_SIGNATURE std::function<int(char* templateCode, char* content)> callback
#else
#define C8Y_CALLBACK_SIGNATURE int (*callback)(char* templateCode, char* content)
#endif

typedef struct Credentials {
	char* tenant;
	char* username;
	char* password;
};

class CumulocityClient {

  public:
    CumulocityClient(Client& networkClient, char* deviceId);

   	void setCallback(C8Y_CALLBACK_SIGNATURE);
	void setKeepAlive(int keepAlive);
	void setDeviceId(char* deviceId);
	void setDeviceId(char* deviceId, char* defaultTemplate);
	void setSupportedOperations(char* operations);
	void getPendingOperations();
	
	bool reconnect();
	bool connect(char* host, char* tenant, char* user, char* password);
	bool connect(char* host, char* tenant, char* user, char* password, char* defaultTemplate);
	void disconnect();

	void setDeviceCredentials(char* tenant, char* user, char* password);
	Credentials getCredentials();
	void retrieveDeviceCredentials();
	bool checkCredentialsReceived();

	void registerDevice(char* deviceName, char* deviceType);
    void createMeasurement(char* fragment, char* series, char* value, char* unit);
	void loop();

  private:
	bool connectClient();
	void callbackHandler(char* topic, byte* payload, unsigned int length);
	void handleOperation();
	void parseCredentials(char* payload);
	char** parseCSV(char* payload);
	void freeCSVElements(char **parsed);
	int countFields( const char *line );

	C8Y_CALLBACK_SIGNATURE;

	PubSubClient _client;

	char* _host;
    Credentials _credentials;
	char* _clientId;
	bool _credentialsReceived;
	int _keepAlive = 600;
	char* _deviceId;
};

#endif
