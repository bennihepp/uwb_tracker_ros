// Adapted by Benjamin Hepp from ArduHDLC library

/*
Arduhdlc is Arduino HDLC library, which can be used over any interface.
Copyright (C) 2015 Jarkko Hautakorpi et al. see LICENSE.txt
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

namespace ait {

  class AITLink {
  public:
    /* HDLC Asynchronous framing */
    /* The frame boundary octet is 01111110, (7E in hexadecimal notation) */
    const static uint8_t FRAME_BOUNDARY_OCTET = 0x7E;

    /* A "control escape octet", has the bit sequence '01111101', (7D hexadecimal) */
    const static uint8_t CONTROL_ESCAPE_OCTET = 0x7D;

    /* If either of these two octets appears in the transmitted data, an escape octet is sent, */
    /* followed by the original data octet with bit 5 inverted */
    const static uint8_t INVERT_OCTET = 0x20;

    /* The frame check sequence (FCS) is a 16-bit CRC-CCITT */
    /* AVR Libc CRC function is _crc_ccitt_update() */
    /* Corresponding CRC function in Qt (www.qt.io) is qChecksum() */
    const static uint16_t CRC16_CCITT_INIT_VAL = 0xFFFF;

    AITLink(size_t max_frame_length = 1024);
    virtual ~AITLink();

    bool sendFrame(const uint8_t* frame_buffer, size_t frame_length);
    void inputReceivedChar(uint8_t data);

    /* User must define a function, that will process the valid received frame */
    /* This function can act like a command router/dispatcher */
    void registerFrameHandler(void (*callback)(void* user_data, const uint8_t* frame_buffer, size_t frame_length), void* user_data);

  protected:
    virtual void handleFrame(const uint8_t* frame_buffer, size_t frame_length);
    /* User must define a function to send a byte throug USART, I2C, SPI etc.*/
    virtual void sendChar(uint8_t data) = 0;

  private:
    uint16_t crcCcittUpdate(uint16_t crc, uint8_t data);

    bool escape_character;
    size_t frame_position;
    // 16bit CRC sum for crcCcittUpdate
    uint16_t frame_checksum;
    size_t max_frame_length;
    uint8_t* receive_frame_buffer;
    void (*handle_frame_callback)(void* user_data, const uint8_t* frame_buffer, size_t frame_length);
    void* callback_user_data;
  };

}
