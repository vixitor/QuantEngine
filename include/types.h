#pragma once
#include "macros.h"
#include <cstdint>
#include <limits>
#include <string>

typedef uint64_t OrderId;
constexpr OrderId OrderId_INVALID = std::numeric_limits<OrderId>::max();
inline std::string orderIdToString(OrderId order_id) {
  if (UNLIKELY(order_id == OrderId_INVALID)) {
    return "INVALID";
  } else {
    return std::to_string(order_id);
  }
}

typedef uint32_t TickerId;
constexpr TickerId TickerId_INVALID = std::numeric_limits<TickerId>::max();
inline std::string tickerIdToString(TickerId ticker_id) {
  if (UNLIKELY(ticker_id == TickerId_INVALID)) {
    return "INVALID";
  } else {
    return std::to_string(ticker_id);
  }
}

typedef uint32_t ClientId;
constexpr ClientId ClientId_INVALID = std::numeric_limits<ClientId>::max();
inline std::string clientIdToString(ClientId client_id) {
  if (UNLIKELY(client_id == ClientId_INVALID)) {
    return "INVALID";
  } else {
    return std::to_string(client_id);
  }
}

typedef int64_t Price;
constexpr Price Price_INVALID = std::numeric_limits<Price>::max();
inline std::string priceToString(Price price) {
  if (UNLIKELY(price == Price_INVALID)) {
    return "INVALID";
  } else {
    return std::to_string(price);
  }
}

typedef uint32_t Quantity;
constexpr Quantity Quantity_INVALID = std::numeric_limits<Quantity>::max();
inline std::string qtyToString(Quantity quantity) {
  if (UNLIKELY(quantity == Quantity_INVALID)) {
    return "INVALID";
  }
  return std::to_string(quantity);
}

typedef uint64_t Priority;
constexpr Priority Priority_INVALID = std::numeric_limits<Priority>::max();
inline std::string priorityToString(Priority priority) {
  if (UNLIKELY(priority == Priority_INVALID)) {
    return "INVALID";
  }
  return std::to_string(priority);
}

enum class Side : int8_t { INVALID = 0, BUY = 1, SELL = -1 };
inline std::string sideToString(Side side) {
  switch (side) {
  case Side::BUY:
    return "BUY";
  case Side::SELL:
    return "SELL";
  case Side::INVALID:
    return "INVALID";
  }
  return "UNKNOWN";
}
constexpr size_t LOG_QUEUE_SIZE = 8 * 1024 * 1024;
constexpr size_t ME_MAX_TICKERS = 8;
constexpr size_t ME_MAX_CLIENT_UPDATES = 256 * 1024;
constexpr size_t ME_MAX_MARKET_UPDATES = 256 * 1024;
constexpr size_t ME_MAX_NUM_CLIENTS = 256;
constexpr size_t ME_MAX_ORDER_IDS = 1024 * 1024;
constexpr size_t ME_MAX_PRICE_LEVELS = 256;