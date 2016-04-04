#pragma once

#include <ait_link/ait_link_impl.h>
#include "uwb_link.h"

#include <iostream>

namespace ait {

  #ifdef __MBED__
  #include "uwb_link_mbed.h"
  #else
  // We need this extra base-class to ensure initialization of AITLinkImpl when initializing UWBLink in UWBLinkImpl
  class UWBLinkImplBase {
  public:
    UWBLinkImplBase()
            : _ait_link(&serial_) {
    }
  protected:
    Serial serial_;
    AITLinkImpl _ait_link;
  };

  class UWBLinkImpl : public UWBLinkImplBase, public UWBLink {
  public:
    UWBLinkImpl(int buffer_size = 1024)
            : UWBLink(&_ait_link, buffer_size) {
      serial_.setReadCallback(boost::bind(std::mem_fun(&UWBLink::inputReceivedChar), this, _1));
    }

    UWBLinkImpl(const std::string& device, unsigned int baud_rate, int buffer_size = 1024)
        : UWBLink(&_ait_link, buffer_size) {
      serial_.setReadCallback(boost::bind(std::mem_fun(&UWBLink::inputReceivedChar), this, _1));
      start(device, baud_rate);
    }

    void start(const std::string& device, unsigned int baud_rate) {
      serial_.open(device, baud_rate);
      serial_.start();
    }
  };
  #endif // __MBED__

}
