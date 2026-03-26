#pragma once
#include "lf_queue.h"
#include "types.h"
#include <sstream>

enum class ClientResponseType : uint8_t {
  INVALID = 0,
  ACCEPTED = 1,
  CANCELED = 2,
  FILLED = 3,
  CANCEL_REJECTED = 4
};
inline std::string clientResponseTypeToString(ClientResponseType type) {
  switch (type) {
  case ClientResponseType::ACCEPTED:
    return "ACCEPTED";
  case ClientResponseType::CANCELED:
    return "CANCELED";
  case ClientResponseType::FILLED:
    return "FILLED";
  case ClientResponseType::CANCEL_REJECTED:
    return "CANCEL_REJECTED";
  case ClientResponseType::INVALID:
    return "INVALID";
  }
  return "UNKNOWN";
}
#pragma pack(push, 1)
class MEClientResponse {
  ClientResponseType type_ = ClientResponseType::INVALID;
  ClientId client_id_ = ClientId_INVALID;
  TickerId ticker_id_ = TickerId_INVALID;
  OrderId client_order_id_ = OrderId_INVALID;
  OrderId market_order_id_ = OrderId_INVALID;
  Side side_ = Side::INVALID;
  Price price_ = Price_INVALID;
  Quantity exec_qty_ = Quantity_INVALID;
  Quantity left_qty_ = Quantity_INVALID;

public:
  MEClientResponse(ClientResponseType type, ClientId client_id, TickerId ticker_id,
                   OrderId client_order_id, OrderId market_order_id, Side side, Price price,
                   Quantity exec_qty, Quantity left_qty)
      : type_(type), client_id_(client_id), ticker_id_(ticker_id),
        client_order_id_(client_order_id), market_order_id_(market_order_id), side_(side),
        price_(price), exec_qty_(exec_qty), left_qty_(left_qty) {}
  MEClientResponse() = default;
  std::string toString() const {
    std::stringstream ss;
    ss << "MEClientResponse"
       << " ["
       << "type:" << clientResponseTypeToString(type_) << " client:" << clientIdToString(client_id_)
       << " ticker:" << tickerIdToString(ticker_id_)
       << " coid:" << orderIdToString(client_order_id_)
       << " moid:" << orderIdToString(market_order_id_) << " side:" << sideToString(side_)
       << " exec_qty:" << qtyToString(exec_qty_) << " left_qty:" << qtyToString(left_qty_)
       << " price:" << priceToString(price_) << "]";
    return ss.str();
  }
};
#pragma pack(pop)
typedef LFQueue<MEClientResponse> ClientResponseLFQueue;
