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


#include "ait_link/ait_link.h"

using namespace ait;

/* 16bit low and high bytes copier */
#define low(x)    ((x) & 0xFF)
#define high(x)   (((x)>>8) & 0xFF)

AITLink::AITLink(size_t max_frame_length)
{
  frame_checksum = CRC16_CCITT_INIT_VAL;
  escape_character = false;
  frame_position = 0;
  this->max_frame_length = max_frame_length;
  receive_frame_buffer = new uint8_t[max_frame_length+1]; // char* ab = (char*)malloc(12);
  handle_frame_callback = NULL;
  callback_user_data = NULL;
}

AITLink::~AITLink()
{
  delete[] receive_frame_buffer;
}

/* Wrap given data in HDLC frame and send it out byte at a time*/
bool AITLink::sendFrame(const uint8_t* frame_buffer, size_t frame_length)
{
  if (frame_length > max_frame_length) {
    return false;
  }

  uint8_t data;
  uint16_t fcs = CRC16_CCITT_INIT_VAL;

  sendChar(FRAME_BOUNDARY_OCTET);

  while (frame_length)
  {
    data = *frame_buffer++;
    fcs = crcCcittUpdate(fcs, data);
    if ((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
    {
      sendChar(CONTROL_ESCAPE_OCTET);
      data ^= INVERT_OCTET;
    }
    sendChar(data);
    --frame_length;
  }
  data = low(fcs);
  if ((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
  {
    sendChar(CONTROL_ESCAPE_OCTET);
    data ^= INVERT_OCTET;
  }
  sendChar(data);
  data = high(fcs);
  if ((data == CONTROL_ESCAPE_OCTET) || (data == FRAME_BOUNDARY_OCTET))
  {
    sendChar(CONTROL_ESCAPE_OCTET);
    data ^= INVERT_OCTET;
  }
  sendChar(data);
  sendChar(FRAME_BOUNDARY_OCTET);

  return true;
}

/* Function to find valid HDLC frame from incoming data */
void AITLink::inputReceivedChar(uint8_t data)
{
  /* FRAME FLAG */
  if (data == FRAME_BOUNDARY_OCTET)
  {
    if (this->escape_character == true)
    {
      this->escape_character = false;
    }
      /* If a valid frame is detected */
    else if ( (this->frame_position >= 2) &&
              ( this->frame_checksum == ((this->receive_frame_buffer[this->frame_position-1] << 8 ) | (this->receive_frame_buffer[this->frame_position-2] & 0xff)) ) )  // (msb << 8 ) | (lsb & 0xff)
    {
      /* Call the user defined function and pass frame to it */
      handleFrame(receive_frame_buffer, this->frame_position - 2);
    }
    this->frame_position = 0;
    this->frame_checksum = CRC16_CCITT_INIT_VAL;
    return;
  }

  if (this->escape_character)
  {
    this->escape_character = false;
    data ^= INVERT_OCTET;
  }
  else if (data == CONTROL_ESCAPE_OCTET)
  {
    this->escape_character = true;
    return;
  }

  receive_frame_buffer[this->frame_position] = data;

  if (this->frame_position >= 2) {
    this->frame_checksum = crcCcittUpdate(this->frame_checksum, receive_frame_buffer[this->frame_position - 2]);
  }

  this->frame_position++;

  if (this->frame_position >= this->max_frame_length)
  {
    this->frame_position = 0;
    this->frame_checksum = CRC16_CCITT_INIT_VAL;
  }
}

void AITLink::registerFrameHandler(void (*callback)(void* user_data, const uint8_t* frame_buffer, size_t frame_length), void* user_data)
{
  handle_frame_callback = callback;
  callback_user_data = user_data;
}

void AITLink::handleFrame(const uint8_t* frame_buffer, size_t frame_length)
{
  if (handle_frame_callback != NULL) {
    (*handle_frame_callback)(callback_user_data, frame_buffer, frame_length);
  }

}

uint16_t AITLink::crcCcittUpdate(uint16_t crc, uint8_t data)
{
  data ^= low(crc);
  data ^= data << 4;

  return (
          ( ((uint16_t)data << 8) | high(crc) ) ^ ((uint8_t)data >> 4) ^ ((uint16_t)data << 3)
  );
}
