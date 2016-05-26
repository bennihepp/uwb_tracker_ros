//
// Simple Serial Port class.
//
// Created by Benjamin Hepp on 02.04.16.
// Copyright (c) 2016 Benjamin Hepp. All rights reserved.
//

#include "ait_link/Serial.h"

using ait::Serial;

Serial::Serial(const std::string &device, unsigned int baud_rate, size_t read_buffer_size)
        : io_(), serial_port_(io_), read_buffer_(new uint8_t[read_buffer_size]) {
  open(device, baud_rate);
}

Serial::Serial(size_t read_buffer_size)
: io_(), serial_port_(io_), read_buffer_(new uint8_t[read_buffer_size]){
}

Serial::~Serial() {
  stop();
  close();
  delete[] read_buffer_;
}

void Serial::setReadCallback(const boost::function<void(uint8_t)> &read_callback) {
  read_callback_ = read_callback;
}

void Serial::open(const std::string &device, unsigned int baud_rate) {
  boost::system::error_code ec;
  serial_port_.open(device, ec);
  if (ec) {
    throw std::runtime_error("Failed to open serial port: " + ec.message());
  }
  serial_port_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
  serial_port_.set_option(boost::asio::serial_port_base::character_size(8));
  serial_port_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
  serial_port_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
  serial_port_.set_option(
          boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
}

void Serial::close() {
  if (serial_port_.is_open()) {
    serial_port_.cancel();
    serial_port_.close();
  }
}

void Serial::start() {
  // Give work to thread before starting it!
  asyncReadSome();
  boost::thread t(boost::bind(&boost::asio::io_service::run, &io_));
  async_thread_.swap(t);
}

void Serial::stop() {
  io_.stop();
  async_thread_.join();
  io_.reset();
}

int Serial::write(char c) {
  return write(reinterpret_cast<const uint8_t *>(&c), 1);
}

int Serial::write(const std::string &buf) {
  return write(reinterpret_cast<const uint8_t *>(buf.c_str()), buf.size());
}

int Serial::write(const uint8_t *buf, int size) {
  if (!serial_port_.is_open()) {
    throw std::runtime_error("Serial port has not been opened");
  }

  boost::system::error_code ec;
  int written = serial_port_.write_some(boost::asio::buffer(buf, size), ec);
  if (ec) {
    throw std::runtime_error("Failed to write to serial port: " + ec.message());
  }
  return written;
}

void Serial::asyncReadSome() {
  if (!serial_port_.is_open()) {
    return;
  }

  serial_port_.async_read_some(
          boost::asio::buffer(read_buffer_, sizeof(read_buffer_)),
          boost::bind(
                  &Serial::onRead,
                  this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred)
  );
}

void Serial::onRead(const boost::system::error_code &ec, size_t bytes_transferred) {
  boost::mutex::scoped_lock look(mutex_);

  if (!ec) {
    for (int i = 0; i < bytes_transferred; ++i) {
      uint8_t c = read_buffer_[i];
      if (!read_callback_.empty()) {
        read_callback_(c);
      }
    }
  }

  asyncReadSome();
}
