/*
 * $Id: user_config.h,v 1.4 2017/04/10 14:44:00 gaijin Exp $
 *
 * Configuration for local set-up (ie:- Access-point ID & PWD).
 */

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

/*
 * GENERAL SETUP
 */
//#define DEBUG		1		// Verbose output. "#undef" to disable.
//#define DEBUG_TERPRE	1		// Zmachine interpreter debug - EXTREMELY VERBOSE!
#undef DEBUG
#undef DEBUG_TERPRE

/*
 * Filenames for SPIFFS data.
 *    Note that default filenames for all files used by frotz (Z-machine) are
 *    defined in src/frotz.h and have not been changed, so take a look there 
 *    if you're finding unexpected files showing up in SPIFFS.
 *
 *    !! NOTE LEADING "/" CHARACTER IS REQUIRED !!
 */
#define G_FILENAME	"/minizork.z3"	// Game-data filename.  Change for different game.
#define S_FILENAME	"/minizork.qzl"	// Saved-game filename.
#define M_FILENAME	"/memory.dat"	// Memory core working copy (internal use).


/*
 * WIFI SETUP
 *
 * Number of times to test WiFi connection status before 
 * giving up and going back into deep-sleep (this is to
 * prevent the battery being flattened by an unavailable
 * access-point, or other, external problems.
 */
#define WIFI_RETRIES	20

/* Assign a static IP address to your ESP8266 */
#define WIFI_IPADDR	{ 192, 168, 1, 4 }
#define WIFI_NETMSK	{ 255, 255, 255, 0 }
#define WIFI_GATEWY	{ 192, 168, 1, 51 }
#define WIFI_DNSSRV	{ 192, 168, 1, 39 }
#define WIFI_CHANNEL	6		// !!You MUST change this!! -- Restart might be unreliable without this set.

#define STA_SSID	"ZorkGlob99"	// !!You MUST change this!!
#define STA_PASS	"*********"	// !!You MUST change this!!
#define STA_TYPE	AUTH_WPA2_PSK


/*
 * MQTT SETUP
 */
// #define PROTOCOL_NAMEv31      /* MQTT version 3.1 compatible with v0.15 */
#define MQTT_HOST	"hazeltonrig.frogthorpe.org"
// #define MQTT_HOST	"broker.hivemq.com"
#define MQTT_PORT	1883
#define MQTT_BUF_SIZE	2048
#define MQTT_KEEPALIVE	120	/* In seconds. */
#define MQCONDEL	150	/* MQTT connection wait delay. */
#define MQ_TOPIC_MAX	256	/* Maximum size in characters. */
#define	TEMPR_SIZE	8	/* Temperature string conv temp storage. */
#define STRBUFF_SIZE	15	/* String buffer size. */

#define MQTT_CLIENT_ID	"Puce_%08X"
// #define MQTT_USER	"Puce_USER"	/* Unused in this version. */
// #define MQTT_PASS	"Puce_PASS"	/* Unused in this version. */

#define TOPIC1		"timestamp"
#define TOPIC2		"Yellow/raw"
#define TOPIC3		"Yellow/LDR"

#define MQTT_RECONNECT_TIMEOUT 	5	/* In Seconds. */

#define CLIENT_SSL_ENABLE
#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE 	1024


/*
 * NTP/TIMEZONE SETUP
 */
#define STYLE_24HR		/* 12/24-hour display setting for clock. */
#define TZ_ABBREV	"JST"	/* Japan Standard Time (Tokyo/Osaka). */
#define NTP_SERVIP	{ 192, 168, 1, 35 }

#endif		// __USER_CONFIG_H__
