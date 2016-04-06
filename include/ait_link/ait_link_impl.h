#pragma once

#include "ait_link.h"

#ifdef __MBED__

#include "ait_link_mbed.h"

#else

#include "Serial.h"

namespace ait {

class AITLinkImpl : public AITLink {
public:
    AITLinkImpl(Serial* serial, uint16_t max_frame_length = 1024)
        : AITLink(max_frame_length), serial_(serial) {
    }

    virtual ~AITLinkImpl() {
    }

    virtual void sendChar(uint8_t data) {
      serial_->write(data);
    }

private:
  Serial* serial_;
};

}

#endif // __MBED__
