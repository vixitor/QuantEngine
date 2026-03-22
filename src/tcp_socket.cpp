#include "../include/tcp_socket.h"

TCPSocket::TCPSocket(Logger& logger) : logger_(logger) {
  send_buffer_ = new char[TCPBufferSize];
  recv_buffer_ = new char[TCPBufferSize];
  recv_callback_ = [this](TCPSocket* socket, Nanos rx_time) {
    defaultRecvCallback(socket, rx_time);
  };
}
void TCPSocket::close() noexcept {
  if (fd_ != -1) {

    ::close(fd_);
    fd_ = -1;
  }
}

TCPSocket::~TCPSocket() {
  close();
  delete[] send_buffer_;
  send_buffer_ = nullptr;
  delete[] recv_buffer_;
  recv_buffer_ = nullptr;
}

auto TCPSocket::connect(const std::string& ip, const std::string& iface, int port,
                        bool is_listening) noexcept -> int {
  close();
  fd_ = createSocket(logger_, ip, iface, port, false, false, is_listening, 0, true);
  inInAddr.sin_addr.s_addr = INADDR_ANY;
  inInAddr.sin_port = htons(port);
  inInAddr.sin_family = AF_INET;
  return fd_;
}

auto TCPSocket::send(const void* data, std::size_t len) noexcept -> void {
  if (len > 0) {
    memcpy(send_buffer_ + next_send_valid_index_, data, len);
    next_send_valid_index_ += len;
  }
}

auto TCPSocket::sendAndRecv() noexcept -> bool {
  char ctrl[CMSG_SPACE(sizeof(timeval))];
  struct cmsghdr* cmsg = (cmsghdr*)&ctrl;
  struct iovec iov;
  iov.iov_base = recv_buffer_ + next_recv_valid_index_;
  iov.iov_len = TCPBufferSize - next_recv_valid_index_;
  msghdr msg;
  msg.msg_control = ctrl;
  msg.msg_controllen = sizeof(ctrl);
  msg.msg_name = &inInAddr;
  msg.msg_namelen = sizeof(inInAddr);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  const auto n_recv = recvmsg(fd_, &msg, MSG_DONTWAIT);
  if (n_recv > 0) {
    next_recv_valid_index_ += n_recv;
    Nanos kernel_time = 0;
    struct timeval time_kernel;
    if (cmsg->cmsg_level == SOL_SOCKET and cmsg->cmsg_type == SCM_TIMESTAMP and
        cmsg->cmsg_len == CMSG_LEN(sizeof(time_kernel))) {
      memcpy(&time_kernel, CMSG_DATA(cmsg), sizeof(time_kernel));
      kernel_time = time_kernel.tv_sec * NANOS_TO_SECS + time_kernel.tv_usec * NANOS_TO_MICROS;
    }
    const auto user_time = getCurrentNanos();
    logger_.log("%:% %() % read socket:% len:% utime:% ktime:% diff:%\n", __FILE__, __LINE__,
                __FUNCTION__, getCurrentTimeStr(&time_str_), fd_, next_recv_valid_index_, user_time,
                kernel_time, (user_time - kernel_time));
    recv_callback_(this, kernel_time);
  }
  if (next_send_valid_index_ > 0) {
    const auto n = ::send(fd_, send_buffer_, next_send_valid_index_, MSG_DONTWAIT | MSG_NOSIGNAL);
    logger_.log("%:% %() % send socket:% len:%\n", __FILE__, __LINE__, __FUNCTION__,
                getCurrentTimeStr(&time_str_), fd_, n);
    ASSERT(n == next_send_valid_index_, "Could not send all the send buffer");
  }
  next_send_valid_index_ = 0;
  return (n_recv > 0);
}

auto TCPSocket::fd() -> const int& { return fd_; }

auto TCPSocket::recv_len() -> const std::size_t& { return next_recv_valid_index_; }

auto TCPSocket::setFd(int fd) -> void {
  close();
  fd_ = fd;
}

auto TCPSocket::setRecvCallback(const std::function<void(TCPSocket* socket, Nanos rx_time)>& func)
    -> void {
  recv_callback_ = func;
}

void TCPSocket::setNextRecvValidIndex(int index) { next_recv_valid_index_ = index; }

void TCPSocket::setNextSendValidIndex(int index) { next_send_valid_index_ = index; }

std::string TCPSocket::getRecvData(int index) {
  if (index == -1) {
    return std::string(recv_buffer_, next_recv_valid_index_);
  }
  return std::string(recv_buffer_, index);
}
std::string TCPSocket::getSendData(int index) {
  if (index == -1) {
    return std::string(send_buffer_, next_send_valid_index_);
  }
  return std::string(send_buffer_, index);
}
