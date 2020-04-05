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

typedef struct Credentials {
	char* tenant;
	char* username;
	char* password;
};

class CumulocityClient {

  public: 
    CumulocityClient(PubSubClient client, char* tenant, char* user, char* password, const char* deviceId);
	bool connect();
	bool connect(char* defaultTemplate);
	void disconnect();
	void setDeviceCredentials(char* user, char* password);
	void registerDevice(char* deviceName, char* deviceType);
	void retrieveDeviceCredentials();
	Credentials getCredentials();
	bool checkCredentialsReceived();
    void createMeasurement(char* fragment, char* series, char* value);
	void loop();
	
  private:
	bool connectClient();
	void callbackHandler(const char* topic, byte* payload, unsigned int length);
	
	PubSubClient _client;
	
	bool _credentialsReceived;
    Credentials _credentials;
	char* _clientId;
	const char* _deviceId;
};

#endif
