#include "macros.h"
#include <cstdint>
#include <vector>
template <typename T> class MemPool final {
private:
  struct Object {
    T object_;
    bool is_free_ = true;
  };
  std::vector<Object> store_;
  std::size_t next_free_index = 0;
  auto updateNextFreeIndex() {
    const auto initial_free_index = next_free_index;
    while (store_[next_free_index].is_free_ == false) {
      next_free_index++;
      if (UNLIKELY(next_free_index == store_.size())) {
        next_free_index = 0;
      }
      if (UNLIKELY(next_free_index == initial_free_index)) {
        FATAL("MemPool out of space");
      }
    }
  }

public:
  explicit MemPool(std::size_t num_elems) : store_(num_elems, {T(), true}) {
    ASSERT(reinterpret_cast<Object*>(&(store_[0].object_)) == &(store_[0]),
           "T should be the first element of Object Block");
  }
  MemPool() = delete;
  MemPool(const MemPool&) = delete;
  MemPool(const MemPool&&) = delete;
  MemPool& operator=(const MemPool&) = delete;
  MemPool& operator=(const MemPool&&) = delete;

  template <typename... Args> T* allocate(Args... args) noexcept {
    auto obj_block = &(store_[next_free_index]);
    ASSERT(obj_block->is_free_ == true,
           "Expect free Object Block at index: " + std::to_string(next_free_index));
    obj_block->is_free_ = false;
    T* ret = &(obj_block->object_);
    ret = new (ret) T(args...);
    updateNextFreeIndex();
    return ret;
  }

  auto deallocate(T* elem) {
    const auto obj_index = reinterpret_cast<Object*>(elem) - &store_[0];
    ASSERT(obj_index >= 0 and static_cast<std::size_t>(obj_index) < store_.size(),
           "Deallocate element does not belong to the MemPool");
    ASSERT(store_[obj_index].is_free_ == false,
           "Expect in-use Object Block at index " + std::to_string(obj_index));
    store_[obj_index].is_free_ = true;
  }
};
