#include "Arduino.h"
#include "CumulocityClient.h"

CumulocityClient::CumulocityClient(WiFiClient client, const char* host, char* tenant, char* user, char* password, const char* deviceId) {

  PubSubClient _client(client);
  _client.setServer(host, 1883);
  
  _deviceId = deviceId;
  
  _credentials.tenant = tenant;
  _credentials.username = user;
  _credentials.password = password;
  
}

bool CumulocityClient::connect() {

  sprintf(_clientId, "d:%s", _deviceId);
  return connectClient();
  
}

bool CumulocityClient::connect(char* defaultTemplate) {

  sprintf(_clientId, "d:%s:%s", _deviceId, defaultTemplate);
  return connectClient();

}
  
bool CumulocityClient::connectClient() {
  
  char* user;
  
  sprintf(user, "%s/%s",_credentials.tenant, _credentials.username);
  
  bool success = _client.connect(_clientId, user, _credentials.password, "s/us", 0, false, "400,c8y_ConnectionEvent,\"Device connection was lost.\"", true);
  
  if (!success) {
	  Serial.print("Unable to connect to Cumulocity as ");
	  Serial.println(user);
  }
  
  return success;
}

void CumulocityClient::disconnect() {
	
	_client.disconnect();
}

void CumulocityClient::retrieveDeviceCredentials() {
	
	_credentialsReceived = false;
	_client.subscribe("s/dcr");
	_client.publish("s/ucr", "");
}

bool CumulocityClient::checkCredentialsReceived() {
	
	return _credentialsReceived;
}

void CumulocityClient::setDeviceCredentials(char* user, char* password) {
	
}

void CumulocityClient::registerDevice(char* deviceName, char* deviceType){
	char* mqttMessage;
	sprintf(mqttMessage, "100,%s,%s", deviceName, deviceType);
	_client.publish("s/us", mqttMessage);
}

void CumulocityClient::createMeasurement() {
	
}
  
