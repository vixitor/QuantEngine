#pragma once
#include "tcp_socket.h"
#include <functional>
#include <sys/epoll.h>
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
class TCPServer {
private:
  int efd_ = -1;
  TCPSocket listener_socket_;
  epoll_event events_[1024];
  std::vector<TCPSocket*> sockets_, receive_sockets_, send_sockets_, disconnected_sockets_;
  std::function<void(TCPSocket*, Nanos)> recv_callback_;
  std::function<void()> recv_finished_callback_;
  std::string time_str_;
  Logger& logger_;
  auto defaultRecvCallback(TCPSocket* socket, Nanos rx_time) noexcept -> void;
  auto defaultRecvFinishedCallback() noexcept -> void;
  auto epoll_add(TCPSocket* socket) -> bool;
  auto epoll_del(TCPSocket* socket) -> bool;

public:
  explicit TCPServer(Logger&);
  auto close() -> void;
  TCPServer() = delete;
  TCPServer(const TCPServer&) = delete;
  TCPServer(const TCPServer&&) = delete;
  TCPServer& operator=(const TCPServer&) = delete;
  TCPServer& operator=(const TCPServer&&) = delete;
  auto listen(const std::string& iface, int port) -> void;
  auto del(TCPSocket* socket) -> void;
  auto poll() noexcept -> void;
  void setRecvCallback(std::function<void(TCPSocket*, Nanos)>);
  void setRecvFinishCallback(std::function<void()>);
  auto sendAndRecv() noexcept -> void;
};