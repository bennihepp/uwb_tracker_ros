#pragma once

#include <BufferedSerial/BufferedSerial.h>

#include "ait_link.h"

#ifdef __MBED__

namespace ait {

  class AITLinkMbed : public AITLink {
    BufferedSerial* _serial;

public:
    AITLinkMbed(BufferedSerial* serial, uint16_t max_frame_length = 1024)
        : AITLink(max_frame_length), _serial(serial) {
    }

    virtual ~AITLinkMbed() {
    }

    virtual void frameHandler(const uint8_t* frame_buffer, size_t frame_length) {
    }

    virtual void sendChar(uint8_t data) {
        _serial->putc(data);
    }
};

}

#endif // __MBED__
