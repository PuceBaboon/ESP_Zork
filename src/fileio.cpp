
/* Jzip V2.1 Infocom/Inform Zcode Format Interpreter
 * --------------------------------------------------------------------
 * Copyright (c) 2000  John D. Holder.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *       
 * --------------------------------------------------------------------
 */

 /*
  * Modified by Louis Davis April, 2012
  * Arduino support.
  *
  */

 /*
  * ESP8266 SPIFFS version.
  *    NOTE:- SPIFFS.begin() is called in ESP_Zork.ino.
  */
#include <FS.h>
#include "user_config.h"
#include "Well_I_do_declare.h"

#include "ztypes.h"

/* Static data */
extern int GLOBALVER;

File game;	/* File Object. */


/*
 * open_story
 *
 * Open game file for read.
 *
 * NOTE - The "memory" core file is left open for reading/writing
 *        on return from this function.
 *
 * NOTE - Both the SD and SPIFFS versions can only have a single
 *        file open at any given time.
 */
void open_story(void) {
    int16_t count;
    uint32_t pos = 0;
    char memory_name[] = M_FILENAME;
    char game_name[] = G_FILENAME;
    uint8_t *stkptr = (uint8_t *) & stack;

    /*
     * Open (or create) memory core file and
     * truncate it.
     */
    game = SPIFFS.open(memory_name, "w+");
    if (game) {
        game.close();	/* Close memory core file. */
    } else {
        esp_osfatal("Could not open memory file(1)");
    }

    /*
     * Open the actual game (Zcode) file and read it
     * in chunks into the memory core file.
     */
    game = SPIFFS.open(game_name, "r");
    if (game) {
        if (!game.seek(0, SeekSet))
            esp_osfatal("Seek in game file failed(1)");

	/*
	 * Chunking loop.
	 */
        while ((count = game.read(stkptr, sizeof(stack))) > 0) {
            game.close();	/* Close game (Zcode) file. */

	    Serial.print(F("."));
            game = SPIFFS.open(memory_name, "a+");	/* Open memory core file. */
            if (!game) {
                esp_osfatal("Failed to open memory file(2)");
            } else {
                if (!game.seek(0, SeekEnd))
                    esp_osfatal("Seek in memory file failed(1) ");
                game.write(stkptr, count);
                game.close();	/* Close memory core file. */
            }

            game = SPIFFS.open(game_name, "r");	/* Re-open game (Zcode) file for reading. */
            if (!game) {
                esp_osfatal(" Failed to open game file(2) ");
            } else {
                pos += count;
                if (!game.seek(pos, SeekSet))
                    esp_osfatal("Rel seek in game file failed(2)");
            }
        }

        game.close();	/* Close game (Zcode) file. */
	Serial.println(F("."));

	/* Finally, open the memory core file for read/write before returning. */
        game = SPIFFS.open(memory_name, "r+");
        if (!game)
            esp_osfatal(" Failed to open memory file(3) ");

    } else {
        esp_osfatal(" Failed to open game file(1) ");
    }
    return;
}                               /* open_story */


/*
 * close_story
 *
 * Close game file if open.
 */
void close_story(void) {
        game.close();
} /* close_story */


/*
 * get_story_size
 *
 * Calculate the size of the game file. Only used for very old games that do not
 * have the game file size in the header.
 */
unsigned int get_story_size(void) {
    return game.size();
}                               /* get_story_size */


/*
 * z_verify
 *
 * Verify game ($verify verb). Add all bytes in game file except for bytes in
 * the game file header.
 */
void z_verify(void) {
    /* Make a conditional jump based on whether the checksum is equal */
    conditional_jump(TRUE);
}


/*
 * z_save
 *
 * Saves data to disk. Returns:
 *     0 = save failed
 *     1 = save succeeded
 */
int z_save(int argc, zword_t table, zword_t bytes, zword_t name) {
    int status = 0;

    /* Get the file name */
    status = 1;

    /* Return result of save to Z-code */
    if (h_type < V4) {
        conditional_jump(status == 0);
    } else {
        store_operand((zword_t) ((status == 0) ? 1 : 0));
    }
    return (status);
}                               /* z_save */


/*
 * z_restore
 *
 * Restore game state from disk. Returns:
 *     0 = restore failed
 *     2 = restore succeeded
 */
int z_restore(int argc, zword_t table, zword_t bytes, zword_t name) {
    int status = 1;

    /* Return result of save to Z-code */
    if (h_type < V4) {
        conditional_jump(status == 0);
    } else {
        store_operand((zword_t) ((status == 0) ? 2 : 0));
    }
    return (status);
}                               /* z_restore */


/*
 * z_save_undo
 *
 * Save the current Z machine state in memory for a future undo. Returns:
 *    -1 = feature unavailable
 *     0 = save failed
 *     1 = save succeeded
 */
void z_save_undo(void) {
    /* If no memory for data area then say undo is not available */
    store_operand((zword_t) - 1);
}                               /* z_save_undo */


/*
 * z_restore_undo
 *
 * Restore the current Z machine state from memory. Returns:
 *    -1 = feature unavailable
 *     0 = restore failed
 *     2 = restore succeeded
 */
void z_restore_undo(void) {
    /* If no memory for data area then say undo is not available */
    store_operand((zword_t) - 1);
}                               /* z_restore_undo */


/*
* read_code_word
*
* Read a word from the instruction stream.
*/
zword_t read_code_word(void) {
    zword_t w;

    w = (zword_t) read_code_byte() << 8;
    w |= (zword_t) read_code_byte();

    return (w);
}                               /* read_code_word */


/*
* read_code_byte
*
* Read a byte from the instruction stream.
*/
zbyte_t read_code_byte(void) {
    zbyte_t value;

    /* Seek to start of page */
    game.seek(pc, SeekSet);

    value = game.read();

    /* Update the PC */
    pc++;

    /* Return byte from page offset */
    return value;
}                               /* read_code_byte */


/*
* read_data_word
*
* Read a word from the data area.
*/
zword_t read_data_word(unsigned long *addr) {
    zword_t w;

    w = (zword_t) read_data_byte(addr) << 8;
    w |= (zword_t) read_data_byte(addr);

    return (w);
}                               /* read_data_word */


void write_data_word(unsigned long *addr, zword_t value) {
    write_data_byte(addr, (zbyte_t) (value >> 8));
    write_data_byte(addr, (zbyte_t) (value));
}                               /* write_data_word */


/*
* read_data_byte
*
* Read a byte from the data area.
*/
zbyte_t read_data_byte(unsigned long *addr) {
    zbyte_t value = 0;

    /* Seek to start of page */
    game.seek(*addr, SeekSet);
    value = game.read();

    /* Update the address */
    (*addr)++;

    return (value);
}                               /* read_data_byte */


void write_data_byte(unsigned long *addr, zbyte_t value) {
    /* Seek to start of page */
    game.seek(*addr, SeekSet);
    game.write(value);

    /* Update the address */
    (*addr)++;
}                               /* write_data_byte */


zbyte_t get_byte(unsigned long offset) {
    unsigned long addr = offset;
    return read_data_byte(&addr);
}


zword_t get_word(unsigned long offset) {
    unsigned long addr = offset;
    return read_data_word(&addr);
}


void set_byte(unsigned long offset, zbyte_t value) {
    unsigned long addr = offset;
    write_data_byte(&addr, value);
}


void set_word(unsigned long offset, zword_t value) {
    unsigned long addr = offset;
    write_data_word(&addr, value);
}
