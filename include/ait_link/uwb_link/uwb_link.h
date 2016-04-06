//
// Simple message protocol for UWB.
//
// Created by Benjamin Hepp on 02.04.16.
// Copyright (c) 2016 Benjamin Hepp. All rights reserved.
//

#pragma once

#include <string.h>
#include <vector>

#include <ait_link/ait_link.h>

namespace ait {

  class UWBMessageBody {
  public:
    virtual int getSize() const = 0;
    virtual void buildMessage(uint8_t* buffer) const = 0;
    virtual bool decodeMessage(const uint8_t* buffer, size_t buffer_size) = 0;
  };

  class UWBMessageString : public UWBMessageBody {
  public:
    UWBMessageString()
            : str_length_(-1), str_(NULL) {
    }

    UWBMessageString(const char* str)
            : str_(str) {
      str_length_ = strlen(str);
    }

    virtual int getSize() const {
      return str_length_ + 1;
    }

    virtual void buildMessage(uint8_t* buffer) const {
      memcpy(buffer, str_, getSize());
    }

    virtual bool decodeMessage(const uint8_t* buffer, size_t buffer_size) {
#ifdef __MBED__
      // TODO: This can lead to a buffer overrun (something like strnlen should be used)
      str_length_ = strlen(reinterpret_cast<const char*>(buffer));
#else
      str_length_ = strnlen(reinterpret_cast<const char*>(buffer), buffer_size);
      if (str_length_ >= buffer_size) {
        return false;
      }
#endif
      str_ = reinterpret_cast<const char*>(buffer);
      return true;
    }

    int getStringLength() const {
      return str_length_;
    }

    const char* getString() const {
      return str_;
    }

  private:
    int str_length_;
    const char* str_;
  };

  struct UWBMessageMultiRange : public UWBMessageBody {
    UWBMessageMultiRange()
            : address(0), remote_address(0) {
    }

    UWBMessageMultiRange(uint8_t address, uint8_t remote_address)
            : address(address), remote_address(remote_address) {
    }

    void clearMeasurements() {
      timestamp_master_request_1.clear();
      timestamp_slave_reply.clear();
      timestamp_master_request_2.clear();
    }

    void addModuleMeasurement(uint64_t timestamp_master_request_1, uint64_t timestamp_slave_reply, uint64_t timestamp_master_request_2) {
      this->timestamp_master_request_1.push_back(timestamp_master_request_1);
      this->timestamp_slave_reply.push_back(timestamp_slave_reply);
      this->timestamp_master_request_2.push_back(timestamp_master_request_2);
    }

    void setSlaveMeasurement(uint64_t timestamp_master_request_1_recv, uint64_t timestamp_slave_reply_send, uint64_t timestamp_master_request_2_recv) {
      this->timestamp_master_request_1_recv = timestamp_master_request_1_recv;
      this->timestamp_slave_reply_send = timestamp_slave_reply_send;
      this->timestamp_master_request_2_recv = timestamp_master_request_2_recv;
    }

    int getNumOfModules() const {
      return timestamp_master_request_1.size();
    }

    virtual int getSize() const {
      return sizeof(uint8_t) + sizeof(address) + sizeof(remote_address) + (getNumOfModules() + 1) * 3 * sizeof(uint64_t);
    }

    virtual void buildMessage(uint8_t* buffer) const {
      // Number of modules and addresses
      uint8_t num_of_modules = static_cast<uint8_t>(getNumOfModules());
      *buffer = num_of_modules;
      ++buffer;
      *buffer = address;
      ++buffer;
      *buffer = remote_address;
      ++buffer;
      // Slave timestamps
      uint64_t* buffer_64 = reinterpret_cast<uint64_t*>(buffer);
      *buffer_64 = timestamp_master_request_1_recv;
      ++buffer_64;
      *buffer_64 = timestamp_slave_reply_send;
      ++buffer_64;
      *buffer_64 = timestamp_master_request_2_recv;
      ++buffer_64;
      // Master and listener timestamps
      for (int i = 0; i < num_of_modules; ++i) {
        *buffer_64 = timestamp_master_request_1[i];
        ++buffer_64;
        *buffer_64 = timestamp_slave_reply[i];
        ++buffer_64;
        *buffer_64 = timestamp_master_request_2[i];
        ++buffer_64;
      }
    }

    virtual bool decodeMessage(const uint8_t* buffer, size_t buffer_size) {
      if (buffer_size < sizeof(uint8_t) + sizeof(address) + sizeof(remote_address)) {
        return false;
      }

      clearMeasurements();

      // Number of modules and addresses
      int num_of_modules = *buffer;
      ++buffer;
      address = *buffer;
      ++buffer;
      remote_address = *buffer;
      ++buffer;
      if (buffer_size < getSize()) {
        return false;
      }
      // Slave timestamps
      const uint64_t* buffer_64 = reinterpret_cast<const uint64_t*>(buffer);
      timestamp_master_request_1_recv = *buffer_64;
      ++buffer_64;
      timestamp_slave_reply_send = *buffer_64;
      ++buffer_64;
      timestamp_master_request_2_recv = *buffer_64;
      ++buffer_64;
      // Master and listener timestamps
      for (int i = 0; i < num_of_modules; ++i) {
        timestamp_master_request_1.push_back(*buffer_64);
        ++buffer_64;
        timestamp_slave_reply.push_back(*buffer_64);
        ++buffer_64;
        timestamp_master_request_2.push_back(*buffer_64);
        ++buffer_64;
      }
      return true;
    }

    uint8_t address;
    uint8_t remote_address;

    uint64_t timestamp_master_request_1_recv;
    uint64_t timestamp_slave_reply_send;
    uint64_t timestamp_master_request_2_recv;
    std::vector<uint64_t> timestamp_master_request_1;
    std::vector<uint64_t> timestamp_slave_reply;
    std::vector<uint64_t> timestamp_master_request_2;
  };

  class UWBMessage {
  public:
    const static uint8_t UWB_MESSAGE_TYPE_NOP = 0x00;
    const static uint8_t UWB_MESSAGE_TYPE_STATUS = 0x01;
    const static uint8_t UWB_MESSAGE_TYPE_MULTI_RANGE = 0x02;

    UWBMessage()
            : type_(UWB_MESSAGE_TYPE_NOP), body_(NULL), part_allocated_(false) {
    }

    UWBMessage(uint8_t type)
            : type_(type), body_(NULL), part_allocated_(false) {
    }

    UWBMessage(uint8_t type, const UWBMessageBody* body)
            : type_(type), body_(body), part_allocated_(false) {
    }

    ~UWBMessage() {
      clearMessageBody();
    }

    uint8_t getType() const {
      return type_;
    }

    const UWBMessageBody* getMessageBody() const {
      return body_;
    }

    void setMessageBody(const UWBMessageBody* body) {
      clearMessageBody();
      body_ = body;
    }

    int getSize() const {
      int size = sizeof(type_);
      if (body_ != NULL) {
        size += body_->getSize();
      }
      return size;
    }

    void buildMessage(uint8_t* buffer) const {
      buffer[0] = type_;
      if (body_ != NULL) {
        buffer += sizeof(type_);
        body_->buildMessage(buffer);
      }
    }

    bool decodeMessage(const uint8_t* buffer, size_t buffer_size) {
      clearMessageBody();
      part_allocated_ = true;
      if (buffer_size < sizeof(type_)) {
        return false;
      }
      type_ = buffer[0];
      buffer += sizeof(type_);
      buffer_size -= sizeof(type_);
      switch (type_) {
        case UWB_MESSAGE_TYPE_NOP: {
          break;
        }
        case UWB_MESSAGE_TYPE_STATUS: {
          UWBMessageString *msg_string = new UWBMessageString();
          if (msg_string->decodeMessage(buffer, buffer_size)) {
            body_ = msg_string;
          } else {
            delete msg_string;
            return false;
          }
          break;
        }
        case UWB_MESSAGE_TYPE_MULTI_RANGE: {
          UWBMessageMultiRange *msg_multi_range = new UWBMessageMultiRange();
          if (msg_multi_range->decodeMessage(buffer, buffer_size)) {
            body_ = msg_multi_range;
          } else {
            delete msg_multi_range;
            return false;
          }
          break;
        }
        default:
          return false;
      }
      return true;
    }

  private:
    void clearMessageBody() {
      if (part_allocated_) {
        delete body_;
        part_allocated_ = false;
      }
      body_ = NULL;
    }

    uint8_t type_;
    const UWBMessageBody* body_;
    bool part_allocated_;
  };

  class UWBLink {
  public:
    UWBLink(AITLink* ait_link, int buffer_size = 1024)
            : handle_message_callback_(NULL), callback_user_data_(NULL),
              ait_link_(ait_link), buffer_size_(buffer_size) {
      buffer_ = new uint8_t[buffer_size];
      ait_link_->registerFrameHandler(&UWBLink::handleFrameWrapper, this);
    }

    virtual ~UWBLink() {
      delete[] buffer_;
    }

    bool sendMessage(const UWBMessage& msg) {
      int size = msg.getSize();
      if (size > buffer_size_) {
        return false;
      }
      msg.buildMessage(buffer_);
      ait_link_->sendFrame(buffer_, size);
      return true;
    }

    void registerMessageHandler(void (*callback)(void* user_data, const UWBMessage& msg), void* user_data) {
      handle_message_callback_ = callback;
      callback_user_data_ = user_data;
    }

    void inputReceivedChar(uint8_t data) {
      ait_link_->inputReceivedChar(data);
    }

  protected:
    virtual void handleMessage(const UWBMessage& msg) {
      if (handle_message_callback_ != NULL) {
        (*handle_message_callback_)(callback_user_data_, msg);
      }
    }

    void (*handle_message_callback_)(void* user_data, const UWBMessage& msg);
    void* callback_user_data_;

  private:
    void handleFrame(const uint8_t* frame_buffer, size_t frame_length) {
      UWBMessage msg;
      if (msg.decodeMessage(frame_buffer, frame_length)) {
        handleMessage(msg);
      } else {
        fprintf(stderr, "Failed to decode UWB message");
      }
    }

    static void handleFrameWrapper(void* user_data, const uint8_t* frame_buffer, size_t frame_length) {
      UWBLink* uwb_link = reinterpret_cast<UWBLink*>(user_data);
      uwb_link->handleFrame(frame_buffer, frame_length);
    }

    AITLink* ait_link_;
    uint8_t* buffer_;
    int buffer_size_;
  };

}
