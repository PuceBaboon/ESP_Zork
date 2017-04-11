/*
 *  $Id: utils.cpp,v 1.1 2017/04/04 09:03:28 gaijin Exp $
 *
 */
#include "utils.h"


/*
 * Add a "yield()" into non-critical delay loops to
 * give the ESP some time for housekeeping operations
 * (just use the normal, unmodified delay() call when
 *  you have very specific timing requirements).
 */
void Ydelay(ulong dcount) {
    yield();
    delay(dcount);
}


/* Convert MAC address to usable string. */
String macToStr(const uint8_t * mac) {
    String result;
    for (int i = 0; i < 6; ++i) {
        result += String(mac[i], 16);
        if (i < 5)
            result += ':';
    }
    return result;
}
