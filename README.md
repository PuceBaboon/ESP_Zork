# ESP_Zork
Port of Louis Davis' Arduino Z-Code Interpreter (AZIP) to the ESP8266.

Louis' AZIP is itself a port of John D. Holder's Jzip (based on ZIP v2.0 sources by Mark Howell and Olaf 'Olsen' Barthel) and, as John's licence requires inclusion of copyright notices, conditions and disclaimer, I have chosen to add the Apache-2.0 licence to this fork, as being closest to John's apparent intent.

My apologies go to Louis, John and all of the other contributors along the way for my merciless, cack-handed butchering of their code.  All of the "Bwaaa-hah-ha!!" moments in this project are mine and not attributable to them.  Sorry folks!

## Important Note
This is a work in progress and, although it does work (as of April 2017), it is effectively unusable.  It is sl..o..o..o...w.  It runs like a dog with three broken legs ...in quicksand.  The terminal I/O could use some TLC.  It's likely that extended use (and, given the current state, this obviously only applies to extreme masochists) will burn through your flash (see "sl..o..o..o...w").

## The Good
* Implemented under PlatformIO (if you're not using PlatformIO yet, go get it!).
* Uses SPIFFS as the filesystem.
* Can host multiple games (not just Zork), so swapping to something like "ztrek" is a trivial, one-line update to the user_config.h file.
* The data directory has other games included (ztrek, alice3, leaptime), mainly as a demonstration of the original jzip's handling of different Z-Code formats (z3, z5, z8).

## The Bad
* Did I mention it was SL..O..O..O...W?
* Constantly reads/writes to SPIFFS.
* Terminal I/O currently screwed up.
* Games other than Zork (which is quite small) tend to be *unbelievably* sl..o..o..o...w (as in wait 5-minutes for a prompt).
* The combination of the terminal and game-size/speed issues make the other included games an interesting curiosity, rather than a real bonus.

## The Ugly
* The terminal I/O handling needs seeing to with a 3kg lump-hammer.  This should be trivial, but the speed issue is at the top of the priority list.
* Game selection (from the games included in the SPIFFS build) should be super-trivial (the directory listing is already there).
* Adding game save and restore functionality back into the code should also be trivial.
* Buffering -- We need to shoehorn the game file into memory.  This should *just* be possible with Zork; probably not with anything bigger.

## General  Implementation and Use
As noted above, this project is implemented using PlatformIO.  This makes (among other things) the creation and loading of the SPIFFS super easy.  All of the game files are in the "data" directory (*not* the src directory) and using `platformio run -t buildfs` and `platformio run -t uploadfs` will create the SPIFFS image and upload it to flash.

Build and load the code with `platformio run -t upload`.

The "data" directory included here should load without problems on ESP8266 boards with 4MB of flash.  Boards with 1MB should still work if you remove some (most) of the games, other than "minizork.z3".  Forget trying to use the older, 512KB ESP-01 board, though (use `platformio boards` to list available targets with flash and memory sizes).

You can find a metric ton of Z-Code games on line.  Use your favourite search engine to look for "interactive fiction zcode", or jump straight to http://www.ifarchive.org/indexes/if-archiveXgamesXzcode.html for a nicely formatted index page for the ifarchive site.  Note that most .z3-.z8 files should work (after a fashion), but .blorb/.zblorb files will not (currently).  Loading lots of games will weigh down your ESP8266 board on the side where the flash chip is and may cause it to become slightly lopsided.  If you find this happens (or the initial load of the selected game file fails), you should remove files one by one and rebuild/reload the SPIFFS image.




