#include "Arduino.h"
#include "CumulocityClient.h"

CumulocityClient::CumulocityClient(WiFiClient client, String host, String tenant, String user, String password) {

  _client = new PubSubClient(client);
  
  _client.setServer(host, 1883);
  //_client.setCallback(callback);
}
  
