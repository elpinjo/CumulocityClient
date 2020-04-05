#include "Arduino.h"
#include "CumulocityClient.h"

CumulocityClient::CumulocityClient(PubSubClient client, char* tenant, char* user, char* password, const char* deviceId) {

  Serial.printf("CumulocityClient(%s, %s, %s)\n", tenant, user, deviceId);
  
  _client = client;
  _deviceId = deviceId;
  
  _credentials.tenant = tenant;
  _credentials.username = user;
  _credentials.password = password;
}

bool CumulocityClient::connect() {

  Serial.println("connect()");
  _clientId = (char*) malloc(strlen(_deviceId) +3);
  strcpy(_clientId, "d:");
  _clientId = strcat(_clientId, _deviceId);
  
  return connectClient();
  
}

bool CumulocityClient::connect(char* defaultTemplate) {

  _clientId = (char *) malloc(strlen(_deviceId) + strlen(defaultTemplate) + 4);
  sprintf(_clientId, "d:%s:%s", _deviceId, defaultTemplate);
  return connectClient();

}
  
bool CumulocityClient::connectClient() {
  
  Serial.println("ConnectClient()");
  
  _client.setCallback(
	[this](const char* topic, byte* payload, unsigned int length){
		this->callbackHandler(topic, payload, length);
	}
  );

  char* user = (char*) malloc(strlen(_credentials.tenant) + strlen(_credentials.username) + 2);
  
  sprintf(user, "%s/%s",_credentials.tenant, _credentials.username);

  bool success = _client.connect(_clientId, user, _credentials.password, "s/us", 0, false, "400,c8y_ConnectionEvent,\"Connections lost.\"", true);

  
  if (!success) {
	  Serial.print("Unable to connect to Cumulocity as ");
	  Serial.println(user);
  } else {
	Serial.println("Connected to cumulocity.");
  }
  
  free(user);
  
  return success;
}

void CumulocityClient::disconnect() {
	
	_client.disconnect();
	free(_clientId);
}

void CumulocityClient::retrieveDeviceCredentials() {
	
	_credentialsReceived = false;
	_client.subscribe("s/dcr");
	_client.publish("s/ucr", "");
}

bool CumulocityClient::checkCredentialsReceived() {
	
	if (_credentialsReceived) {
		_client.unsubscribe("s/dcr");
	} else {
		_client.publish("s/ucr", "");
		_client.loop();
	}
	
	return _credentialsReceived;
}

Credentials CumulocityClient::getCredentials() {
	
	return _credentials;
}

void CumulocityClient::setDeviceCredentials(char* user, char* password) {
	_credentials.username = user;
	_credentials.password = password;
}

void CumulocityClient::registerDevice(char* deviceName, char* deviceType){
	char* mqttMessage = (char*) malloc(strlen(_deviceId) + strlen(deviceType)+6);
	sprintf(mqttMessage, "100,%s,%s", deviceName, deviceType);
	_client.publish("s/us", mqttMessage);
}

void CumulocityClient::createMeasurement(char* fragment, char* series, char* value) {
	
	char* mqttMessage;
	sprintf(mqttMessage, "200,%s,%s,%s", fragment, series, value);
	_client.publish("s/us", mqttMessage);
	
}

void CumulocityClient::loop() {

  _client.loop();
}

void CumulocityClient::callbackHandler(const char* topic, byte* payload, unsigned int length) {
	
	Serial.printf("callbackHandler(topic: %s, payload: %s)\n", topic, payload);
	
	//int firstComma = 
	char myPayload[length+1];
	
	strncpy(myPayload, (char*)payload,length);
	myPayload[length] = '\0';
	
	Serial.printf("Input length = %d, payload = %d\n", length, strlen(myPayload));
	
	if (strcmp(topic, "s/dcr") == 0 && length > 0) {
		Serial.print("Device credentials received: ");
		Serial.println(myPayload);
		_credentialsReceived = true;
	}
	
}
  
