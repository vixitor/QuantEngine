#pragma once
#include "lf_queue.h"
#include "macros.h"
#include "thread_utils.h"
#include "time_utils.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
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
  void pushValue(const LogElement&);
  void pushValue(const char value);
  void pushValue(const char* value);
  void pushValue(const int value);
  void pushValue(const long value);
  void pushValue(const long long value);
  void pushValue(const unsigned value);
  void pushValue(const unsigned long value);
  void pushValue(const unsigned long long value);
  void pushValue(const float value);
  void pushValue(const double value);
  void pushValue(const std::string& value);
  template <typename T, typename... Args> void log(const char* s, const T& value, Args... args) {
    while (*s) {
      if (*s == '%') {
        if (UNLIKELY(*(s + 1) == '%')) {
          s++;
        } else {
          pushValue(value);
          log(s + 1, args...);
          return;
        }
      }
      pushValue(*s);
      s++;
    }
    FATAL("extra arguments provided to log()");
  }
  void log(const char* s);
};
