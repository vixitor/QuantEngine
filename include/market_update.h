#pragma once
#include "lf_queue.h"
#include "types.h"
#include <sstream>

enum class MarketUpdateType : uint8_t { INVALID = 0, ADD = 1, MODIFY = 2, CANCEL = 3, TRADE = 4 };
inline std::string marketUpdateTypeToString(MarketUpdateType type) {
  switch (type) {
  case MarketUpdateType::ADD:
    return "ADD";
  case MarketUpdateType::MODIFY:
    return "MODIFY";
  case MarketUpdateType::CANCEL:
    return "CANCEL";
  case MarketUpdateType::TRADE:
    return "TRADE";
  case MarketUpdateType::INVALID:
    return "INVALID";
  }
  return "UNKNOWN";
}
#pragma pack(push, 1)
class MEMarketUpdate {
  MarketUpdateType type_ = MarketUpdateType::INVALID;
  OrderId order_id_ = OrderId_INVALID;
  TickerId ticker_id_ = TickerId_INVALID;
  Side side_ = Side::INVALID;
  Price price_ = Price_INVALID;
  Quantity quantity_ = Quantity_INVALID;
  Priority priority_ = Priority_INVALID;

public:
  MEMarketUpdate() = default;
  MEMarketUpdate(MarketUpdateType type, OrderId order_id, TickerId ticker_id, Side side,
                 Price price, Quantity quantity, Priority priority)
      : type_(type), order_id_(order_id), ticker_id_(ticker_id), side_(side), price_(price),
        quantity_(quantity), priority_(priority) {}
  auto toString() const {
    std::stringstream ss;
    ss << "MEMarketUpdate"
       << " ["
       << " type:" << marketUpdateTypeToString(type_) << " ticker:" << tickerIdToString(ticker_id_)
       << " oid:" << orderIdToString(order_id_) << " side:" << sideToString(side_)
       << " qty:" << qtyToString(quantity_) << " price:" << priceToString(price_)
       << " priority:" << priorityToString(priority_) << "]";
    return ss.str();
  }
};
#pragma pack(pop)

typedef LFQueue<MEMarketUpdate> MEMarketUpdateLFQueue;