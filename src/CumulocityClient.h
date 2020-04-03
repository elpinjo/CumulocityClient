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
#include <WiFiClient.h>
#include <PubSubClient.h>

class CumulocityClient {

  public: 
    CumulocityClient(WiFiClient client, String host, String tenant, String user, String password);
    createMeasurement();
  private:
    String _host;
    String _tenant;
    String _user;
    String _password;
    PubSubClient _client;
}

#endif
