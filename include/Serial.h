//
// Simple Serial Port class.
//
// Created by Benjamin Hepp on 02.04.16.
// Copyright (c) 2016 Benjamin Hepp. All rights reserved.
//

#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace ait {

  class Serial {
  public:

    Serial(const std::string &device, unsigned int baud_rate, size_t read_buffer_size = 8 * 1024);
    Serial(size_t read_buffer_size = 8 * 1024);
    virtual ~Serial();

    void setReadCallback(const boost::function<void(uint8_t)> &read_callback);

    void open(const std::string &device, unsigned int baud_rate);
    void close();
    void start();
    void stop();

    int write(char c);
    int write(const std::string &buf);
    int write(const uint8_t *buf, int size);

    void asyncReadSome();

  private:
    void onRead(const boost::system::error_code &ec, size_t bytes_transferred);

    boost::asio::io_service io_;
    boost::asio::serial_port serial_port_;
    boost::thread async_thread_;
    boost::mutex mutex_;
    uint8_t* read_buffer_;
    boost::function<void(uint8_t)> read_callback_;
  };

}
