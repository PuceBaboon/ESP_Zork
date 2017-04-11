/*
 *    $Id: ESP_Zork.ino,v 1.8 2017/04/10 14:43:15 gaijin Exp $
 *
 * Zork for the ESP8266.
 *
 * ESP8266-specific includes first.
 */

/* Azip Arduino Zcode Interpreter Program
 * --------------------------------------------------------------------
 * Derived from John D. Holder's Jzip V2.1 source code
 * http://jzip.sourceforge.net/
 * --------------------------------------------------------------------
 * 
 * Arduino version created by Louis Davis April, 2012
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <FS.h>
#include "user_config.h"
#include "Well_I_do_declare.h"
#include "utils.h"

#include "ztypes.h"


/*
 * Calls to exit() from AZIP/JZip indicate a fatal
 * error of some sort, so notify the user and
 * initiate a restart of the ESP8266.
 */
//extern "C" void esp_osfatal(const char *);
void esp_osfatal(const char *s) {
    Serial.print("\n\tFATAL ERROR: ");
    Serial.print(s);
    Serial.println("  - Restarting...");
    Ydelay(10000);
    ESP.reset();
}


void setup() {
    Serial.begin(115200);

    Ydelay(200);
    Serial.println(F("\n"));    // <CR> past boot messages.
#ifdef DEBUG
    Serial.print(F("Reset Info: "));
    Serial.println(ESP.getResetInfo());
    Serial.println(F("\n"));
#endif

    /* 
     * SPIFFS initialization. 
     *
     *    NOTE - We must have a filesystem for this application, so
     *           a failure at this point is fatal and will trigger
     *           a restart of the ESP8266.
     */
    Serial.println("Starting SPIFFS");
    if (!SPIFFS.begin()) {
        Serial.println("ERROR initializing SPIFFS - Restarting...");
        Ydelay(5000);
        ESP.reset();            /* Can't continue without filesystem. */
    }

    /*
     * If we don't find an existing game file, we'll assume this is a
     * a first-time boot and trigger a format of the filesystem.
     */
    if (!SPIFFS.exists(G_FILENAME)) {
        Serial.print("\n\n\t>>>   Game file not found (");
        Serial.print(G_FILENAME);
        Serial.println(")   <<<");
        Serial.println("\n\nFormatting SPIFFS...   ");
/*        if (!SPIFFS.format()) {
            Serial.println("\n\t!! ERROR: formatting SPIFFS !!");
            Ydelay(500);
            ESP.reset();
        }
        Serial.println("Done.");
        Serial.println
            ("\n\n\tUpload games using \"pio run -t uploadfs\" and then restart.");
        while (1)
            Ydelay(100);        // Can't continue without a game file.
*/
    }


    FSInfo fs_info;
    if (SPIFFS.info(fs_info)) {
        Serial.print(F("SPIFFS tot size (bytes): "));
        Serial.println(fs_info.totalBytes);
        Serial.print(F("SPIFFS bytes used:       "));
        Serial.println(fs_info.usedBytes);
    }
    Serial.println("\nFound files:-");
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
        Serial.print(dir.fileName());
        Serial.print("\t");
        File f = dir.openFile("r");
        Serial.println(f.size());
        f.close();
    }
    Ydelay(5000);               /* Give the user time to read the SPIFFS stats. */

    /* Initialize WiFi and MQTT connections. */
//    WiFisetup();


    Serial.println("\n\nPress \"Enter\" to start...");

    while (!Serial.available()) {
    }
    Serial.read();

    Serial.print(F("\nOpening game file: "));
    Serial.print(G_FILENAME);
    Serial.println(F("..."));
    open_story();

    Serial.println(F("Executing configure..."));
    configure(V1, V8);

    Serial.println(F("Starting game..."));
    initialize_screen();

#ifdef DEBUG
    Serial.println(F("Calling z_restart()."));
#endif
    z_restart();
}


/*
 * main --  Taken from original jzip.c.
 *
 * Prepare and run the game.
 *
 */
void loop() {
    // Handle OTA update requests
    ArduinoOTA.handle();

#ifdef DEBUG
    Serial.println(F("In loop(), calling interpret()..."));
#endif
    interpret();

    /*
     * If we drop out of interpret(), it's because the "halt"
     * flag has been set, so we're going to restart the ESP.
     */
    Serial.println(F("\n\tHalting..."));
    Ydelay(4000);
    ESP.reset();

}   /* main */
