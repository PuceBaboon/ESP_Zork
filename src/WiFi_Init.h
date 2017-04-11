/*
 * $Id: WiFi_Init.h,v 1.1 2017/04/04 09:03:28 gaijin Exp $
 * 
 * WiFi and MQTT initialization, plus OTA handler code.
 */
#ifndef __WIFI_INIT_H__
#define __WIFI_INIT_H__

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "utils.h"
#include "user_config.h"

char clidbuff[STRBUFF_SIZE];

void mqtt_callback(char *intopic, byte * inpayload, unsigned int length);
boolean mqtt_reconnect();
boolean sendmqttMsg(char *topictosend, String payload);
void WiFisetup();

/* NOTE:- All of these values are configured in user_config.h.  */
const byte ip[] = WIFI_IPADDR;
const byte gateway[] = WIFI_GATEWY;
const byte netmask[] = WIFI_NETMSK;
const byte dnssrv[] = WIFI_DNSSRV;


/* ----------------- MQTT ---------------- */
/*
 * Change the defined MQTT packet size and keepalive
 * to larger values, as recommended by Theo.
 *
 * Note that the PubSubClient library should be included
 * -after- these redefines, not before.
 */
#ifdef MQTT_MAX_PACKET_SIZE
#undef MQTT_MAX_PACKET_SIZE
#endif
#define MQTT_MAX_PACKET_SIZE 1024
#ifdef MQTT_KEEPALIVE
#undef MQTT_KEEPALIVE
#endif
#define MQTT_KEEPALIVE 120
#include <PubSubClient.h>

String payload;
char buff[MQ_TOPIC_MAX];

unsigned long startMillis = 0;


#endif     // __WIFI_INIT_H__
