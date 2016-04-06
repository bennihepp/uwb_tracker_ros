#pragma once

#include <ait_link/ait_link_mbed.h>
#include "uwb_link.h"

#ifdef __MBED__
namespace ait {

  class UWBLinkMbed : public UWBLink {
    AITLinkMbed ait_link_;

public:
    UWBLinkMbed(BufferedSerial* serial, int buffer_size = 1024)
        : UWBLink(&ait_link_, buffer_size), ait_link_(serial) {
    }
};

}

#endif // __MBED__
