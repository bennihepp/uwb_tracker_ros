//
// HDLC based serial communication class.
//
// Adapted by Benjamin Hepp from ArduHDLC library (see LICENSE.txt)
// Original work Copyright (c) 2015 Jarkko Hautakorpi et al.
// Modified work Copyright (c) 2016 Benjamin Hepp.

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
