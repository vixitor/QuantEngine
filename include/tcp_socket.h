#pragma once
#include "logger.h"
#include "socket_utils.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <type_traits>
#include <unistd.h>
constexpr size_t TCPBufferSize = 64 * 1024 * 1024;
class TCPSocket {
private:
  int fd_ = -1;
  char* send_buffer_ = nullptr;
  std::size_t next_send_valid_index_ = 0;
  char* recv_buffer_ = nullptr;
  std::size_t next_recv_valid_index_ = 0;
  bool send_disconnected_ = false;
  bool recv_disconnected_ = false;
  struct sockaddr_in inInAddr;
  std::function<void(TCPSocket* s, Nanos rx_time)> recv_callback_;
  std::string time_str_;
  Logger& logger_;
  void defaultRecvCallback(TCPSocket* socket, Nanos rx_time) noexcept {
    logger_.log("%:% %() % TCPSocket::defaultRecvCallback() socket:% len:% rx:%\n", __FILE__,
                __LINE__, __FUNCTION__, getCurrentTimeStr(&time_str_), socket->fd_,
                socket->next_recv_valid_index_, rx_time);
  }

public:
  explicit TCPSocket(Logger&);
  void close() noexcept;
  ~TCPSocket();
  TCPSocket() = delete;
  TCPSocket(const TCPSocket&) = delete;
  TCPSocket(const TCPSocket&&) = delete;
  TCPSocket& operator=(const TCPSocket&) = delete;
  TCPSocket& operator=(const TCPSocket&&) = delete;
  auto connect(const std::string& ip, const std::string& iface, int port,
               bool is_listening) noexcept -> int;
  auto send(const void* data, std::size_t len) noexcept -> void;
  auto sendAndRecv() noexcept -> bool;
  auto fd() -> const int&;
  auto recv_len() -> const std::size_t&;
  auto setFd(int) -> void;
  auto setRecvCallback(const std::function<void(TCPSocket* socket, Nanos rx_time)>& func) -> void;
  std::string getRecvData(int index = -1);
  std::string getSendData(int index = -1);
  void setNextRecvValidIndex(int index);
  void setNextSendValidIndex(int index);
};
