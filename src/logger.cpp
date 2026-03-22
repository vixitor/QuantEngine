#include "../include/logger.h"

auto Logger::flushQueue() noexcept {
  while (running_ == true) {
    for (auto next = queue_.getNextToRead(); queue_.size() && next; next = queue_.getNextToRead()) {
      switch (next->type_) {
      case LogType::CHAR:
        file_ << next->u_.c;
        break;
      case LogType::INTEGER:
        file_ << next->u_.i;
        break;
      case LogType::LONG_INTEGER:
        file_ << next->u_.l;
        break;
      case LogType::LONG_LONG_INTEGER:
        file_ << next->u_.ll;
        break;
      case LogType::UNSIGNED_INTEGER:
        file_ << next->u_.u;
        break;
      case LogType::UNSIGNED_LONG_INTEGER:
        file_ << next->u_.ul;
        break;
      case LogType::UNSIGNED_LONG_LONG_INTEGER:
        file_ << next->u_.ull;
        break;
      case LogType::FLOAT:
        file_ << next->u_.f;
        break;
      case LogType::DOUBLE:
        file_ << next->u_.d;
        break;
      }
      queue_.updataReadIndex();
    }
    using namespace std::literals::chrono_literals;
    std::this_thread::sleep_for(1ms);
  }
}

Logger::Logger(const std::string& file_name) : file_name_(file_name), queue_(LOG_QUEUE_SIZE) {
  file_.open(file_name_);
  ASSERT(file_.is_open() == true, "Could not open log file: " + file_name_);
  logger_thread_ = createAndStartThread(-1, "Logger", [this]() { flushQueue(); });
}

Logger::~Logger() { 
    std::cout<<"Flushing and closing logger for " << file_name_<<std::endl;
    while(queue_.size()){
        using namespace std::literals::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    running_ = false;
    logger_thread_->join();
    file_.close();
}

void Logger::pushValue(const LogElement& log_element) {
    *(queue_.getNextToWriteTo()) = log_element;
    queue_.updateWriteIndex();
}

void Logger::pushValue(const char value) { 
    pushValue(LogElement{LogType::CHAR, {.c = value}});
}

void Logger::pushValue(const char *value){ 
    while(*value){
        pushValue(*value);
        value++;
    }
}

void Logger::pushValue(const int value) { 
    pushValue(LogElement{LogType::INTEGER, {.i = value}});
}

void Logger::pushValue(const long value) { 
    pushValue(LogElement{LogType::LONG_INTEGER, {.l = value}});
}

void Logger::pushValue(const long long value) { 
    pushValue(LogElement{LogType::LONG_LONG_INTEGER, {.ll = value}});
}

void Logger::pushValue(const unsigned value) { 
    pushValue(LogElement{LogType::UNSIGNED_INTEGER, {.u = value}});
}

void Logger::pushValue(const unsigned long value) { 
    pushValue(LogElement{LogType::UNSIGNED_LONG_INTEGER, {.ul = value}});
}

void Logger::pushValue(const unsigned long long value) { 
    pushValue(LogElement{LogType::UNSIGNED_LONG_LONG_INTEGER, {.ull = value}});
}

void Logger::pushValue(const float value) { 
    pushValue(LogElement{LogType::FLOAT, {.f = value}});
}

void Logger::pushValue(const double value) { 
    pushValue(LogElement{LogType::DOUBLE, {.d = value}});
}

void Logger::pushValue(const std::string& value) { 
    pushValue(value.c_str());
}

void Logger::log(const char* s) {
    while(*s){
        if(*s == '%') { 
            if(UNLIKELY(*(s + 1) == '%')) { 
                s++;
            }
            else {
                FATAL("missing args to log()");
            }
        }
        pushValue(*s);
        s++;
    }
}
