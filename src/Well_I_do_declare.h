/*
 * $Id: Well_I_do_declare.h,v 1.2 2017/04/07 06:28:20 gaijin Exp $
 * 
 */
#ifndef __WIDD_H__
#define __WIDD_H__

#include "ztypes.h"


/* ESP_Zork */
void esp_osfatal(const char *s);


/* jzip.c */
void configure(zbyte_t min_version, zbyte_t max_version);


/* WiFi_Init */
void mqtt_callback(char *intopic, byte * inpayload, unsigned int length);
boolean mqtt_reconnect();
boolean sendmqttMsg(char *topictosend, String payload);
void WiFisetup();


/* Utils */
void Ydelay(ulong dcount);
String macToStr(const uint8_t * mac);


#endif     // __WIDD_H__
