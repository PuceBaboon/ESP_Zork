/*
 * $Id: WiFi_Init.cpp,v 1.2 2017/04/08 14:52:27 gaijin Exp $
 * 
 * WiFi and MQTT initialization, plus OTA handler code.
 */
#include "WiFi_Init.h"


WiFiClient wifiClient;
PubSubClient client(MQTT_HOST, MQTT_PORT, mqtt_callback, wifiClient);


/*
 * MQTT callback routine for handling incoming messages from server.
 */
void mqtt_callback(char *intopic, byte * inpayload, unsigned int length) {
    String receivedpayload;

    for (int i = 0; i < length; i++) {
        receivedpayload += (char) inpayload[i];
    }
#ifdef DEBUG
    Serial.print("MQTT topic: ");
    Serial.print(intopic);
    Serial.print(" => ");
    Serial.println(receivedpayload);
#endif
}


/*
 * MQTT reconnect loop for those occasions when the client has
 * been unexpectedly disconnected.
 */
boolean mqtt_reconnect() {
    if (client.connect((char *) clidbuff)) {
        client.subscribe(TOPIC1);       /* Defined in user_config.h. */
#ifdef DEBUG
        Serial.println("MQTT connected.");
#endif
    } else {
#ifdef DEBUG
        Serial.print("MQTT failed, rc=");
        Serial.println(client.state());
#endif
    }
    return client.connected();
}


/*
 * MQTT publish function.
 */
boolean sendmqttMsg(char *topictosend, String payload) {
    if (client.connected()) {
        unsigned int msg_length = payload.length();
#ifdef DEBUG
        Serial.print("Publishing topic + payload:  ");
        Serial.print(topictosend);
        Serial.print(" + ");
        Serial.print(payload);
        Serial.print(" (Length: ");
        Serial.print(msg_length);
        Serial.println(")");
#endif
        byte *p = (byte *) malloc(msg_length);
        memcpy(p, (char *) payload.c_str(), msg_length);

        if (client.publish(topictosend, p, msg_length, 1)) {
#ifdef DEBUG
            Serial.println("Published Successfully.");
#endif
            free(p);
            return 1;
        } else {
#ifdef DEBUG
            Serial.print("Publish Failed.");
#endif
            free(p);
            return 0;
        }
    }
}


void WiFisetup() {
    int conn_tries = 0;

#ifdef DEBUG
    Serial.print(F("\nReset Info: "));
    Serial.println(ESP.getResetInfo());
#endif

    /* Get a unique client-ID for this ESP8266. */
    sprintf(clidbuff, MQTT_CLIENT_ID, ESP.getChipId());

#ifdef DEBUG
    Serial.print(F("\nClientID: "));
    Serial.println(clidbuff);
    Serial.println();
    Serial.print(F("Connecting to "));
    Serial.println(STA_SSID);
#endif


    /*
     * An open-ended loop  here will flatten the battery on
     * the sensor if, for instance, the access point is 
     * down, so limit our connection attempts.
     */
    Serial.print("WiFi Stat: ");
    Serial.println(WiFi.status());      // Reputed to fix slow-start problem.
    WiFi.mode(WIFI_STA);
    WiFi.config(IPAddress(ip), IPAddress(gateway),
                IPAddress(netmask), IPAddress(dnssrv));
    WiFi.begin(STA_SSID, STA_PASS, WIFI_CHANNEL, NULL);
    yield();

    while ((WiFi.status() != WL_CONNECTED)
           && (conn_tries++ < WIFI_RETRIES)) {
        Ydelay(100);
#ifdef DEBUG
        Serial.print(".");
#endif
    }
    if (conn_tries == WIFI_RETRIES) {
#ifdef DEBUG
        Serial.println(F("No WiFi connection ...sleeping."));
#endif
        ESP.reset();
    }

#ifdef DEBUG
    Serial.println();
//    WiFi.printDiag(Serial);	// Quite verbose.
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());
#endif


    /* MQTT initialization - YELLOW */
    client.setServer(MQTT_HOST, MQTT_PORT);
    client.setCallback(mqtt_callback);



    /* OTA (Over The Air) code.  DO NOT DELETE!! */
/* *INDENT-OFF* */
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
/* *INDENT-ON* */
    /* End of OTA code. */
}
