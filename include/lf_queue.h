#pragma once
#include <atomic>
#include <cstddef>
#include <vector>
template <typename T> class LFQueue {
private:
  std::vector<T> store_;
  std::atomic<std::size_t> next_read_index_ = {0};
  std::atomic<std::size_t> next_write_index_ = {0};
  std::atomic<std::size_t> num_elements_ = {0};

public:
  LFQueue(std::size_t num_elem) : store_(num_elem, T()) {}
  LFQueue() = delete;
  LFQueue(const LFQueue&) = delete;
  LFQueue(const LFQueue&&) = delete;
  LFQueue& operator=(const LFQueue&) = delete;
  LFQueue& operator=(const LFQueue&&) = delete;
  auto getNextToWriteTo() noexcept { return &(store_[next_write_index_]); }
  auto updateWriteIndex() noexcept {
    next_write_index_ = (next_write_index_ + 1) % store_.size();
    num_elements_ += 1;
  }
  auto getNextToRead() const noexcept -> const T* {
    return (next_read_index_ == next_write_index_ ? nullptr : &(store_[next_read_index_]));
  }
  auto updataReadIndex() noexcept {
    next_read_index_ = (next_read_index_ + 1) % store_.size();
    num_elements_ -= 1;
  }
  auto size() const noexcept { return num_elements_.load(); }
};