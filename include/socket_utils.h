#pragma once
#include "logger.h"
#include <arpa/inet.h>
#include <cctype>
#include <fcntl.h>
#include <ifaddrs.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_set>

constexpr int MaxTCPServerBacklog = 1024;
auto getIfaceIP(const std::string& iface) -> std::string;
auto setNonBlocking(int fd) -> bool;
auto setNoDelay(int fd) -> bool;
auto setSOTimestamp(int fd) -> bool;
auto wouldBlock() -> bool;
auto setMcastTTL(int fd, int ttl) noexcept -> bool;
auto setTTL(int fd, int ttl) -> bool;
auto join(int fd, const std::string& ip, const std::string& iface, int port) -> bool;
auto createSocket(Logger& logger, const std::string& t_ip, const std::string& iface, int port,
                  bool is_udp, bool is_blocking, bool is_listening, int ttl,
                  bool needs_so_timestamp) -> int;