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
    CumulocityClient(Client& networkClient, char* deviceId);
	bool reconnect();
	bool connect(char* host, char* tenant, char* user, char* password);
	bool connect(char* host, char* tenant, char* user, char* password, char* defaultTemplate);
	void disconnect();
	void setDeviceCredentials(char* tenant, char* user, char* password);
	void registerDevice(char* deviceName, char* deviceType);
	void retrieveDeviceCredentials();
	Credentials getCredentials();
	bool checkCredentialsReceived();
    void createMeasurement(char* fragment, char* series, char* value, char* unit);
	void setKeepAlive(int keepAlive);
	void setDeviceId(char* deviceId);
	void setDeviceId(char* deviceId, char* defaultTemplate);
	void loop();

  private:
	bool connectClient();
	void callbackHandler(const char* topic, byte* payload, unsigned int length);
	void parseCredentials(char* payload);
	char** parseCSV(char* payload);
	void freeCSVElements(char **parsed);
	int countFields( const char *line );

	PubSubClient _client;

	char* _host;
    Credentials _credentials;
	char* _clientId;
	bool _credentialsReceived;
	int _keepAlive = 600;
	char* _deviceId;
};

#endif
