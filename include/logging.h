#pragma once
#include "lf_queue.h"
#include "macros.h"
#include "thread_utils.h"
#include "time_utils.h"
#include <cstdio>
#include <fstream>
#include <string>
#include <thread>

constexpr size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024;
enum class LogType : int8_t {
  CHAR = 0,
  INTEGER = 1,
  LONG_INTEGER = 2,
  LONG_LONG_INTEGER = 3,
  UNSIGNED_INTEGER = 4,
  UNSIGNED_LONG_INTEGER = 5,
  UNSIGNED_LONG_LONG_INTEGER = 6,
  FLOAT = 7,
  DOUBLE = 8
};

struct LogElement {
  LogType type_ = LogType::CHAR;
  union {
    char c;
    int i;
    long l;
    long long ll;
    unsigned u;
    unsigned long ul;
    unsigned long long ull;
    float f;
    double d;
  } u_;
};

class Logger {
private:
  std::string file_name_;
  std::ofstream file_;
  LFQueue<LogElement> queue_;
  std::atomic<bool> running_ = {true};
  std::thread* logger_thread_ = nullptr;

public:
  explicit Logger(const std::string&);
  ~Logger();
  Logger() = delete;
  Logger(const Logger&) = delete;
  Logger(const Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(const Logger&&) = delete;
  auto flushQueue() noexcept;
  auto pushValue(const LogElement&) -> void ;
  auto pushValue(const char value);
  auto pushValue(const char *value) -> void;
  auto pushValue(const int value) -> void;
  auto pushValue(const long value);
  auto pushValue(const long long value);
  auto pushValue(const unsigned value);
  auto pushValue(const unsigned long value);
  auto pushValue(const unsigned long long value);
  auto pushValue(const float value);
  auto pushValue(const double value);
  auto pushValue(const std::string& value) -> void;
  template <typename T,typename... Args> 
  auto log(const char* s,const T &value,Args... args) -> void;
  auto log(const char* s) -> void;
};

