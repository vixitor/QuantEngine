#pragma once
#include "client_request.h"
#include "client_response.h"
#include "logger.h"
#include "market_update.h"
#include "matching_engine.h"
#include "me_order.h"
#include "mem_pool.h"
#include "types.h"
class MatchingEngine;
class MEOrderBook {
  TickerId ticker_id_ = TickerId_INVALID;
  MatchingEngine* matching_engine_ = nullptr;
  ClientOrderHashMap cid_oid_to_order_;
  MemPool<MEOrdersAtPrice> orders_at_price_pool_;
  MEOrdersAtPrice* bids_by_price_ = nullptr;
  MEOrdersAtPrice* asks_by_price_ = nullptr;
  OrdersAtPriceHashMap price_orders_at_price_;
  MemPool<MEOrder> order_pool_;
  MEClientResponse client_response_;
  MEMarketUpdate market_update_;
  OrderId next_market_order_id_ = 1;
  std::string time_str_;
  Logger* logger_ = nullptr;

public:
  MEOrderBook(TickerId, Logger*, MatchingEngine*);
  ~MEOrderBook();
  MEOrderBook() = delete;
  MEOrderBook(const MEOrderBook&) = delete;
  MEOrderBook(const MEOrderBook&&) = delete;
  MEOrderBook& operator=(const MEOrderBook&) = delete;
  MEOrderBook& operator=(const MEOrderBook&&) = delete;
  std::string toString(bool, bool);
  OrderId generateNewMarketOrderId();
  Price priceToIndex(Price);
  MEOrdersAtPrice* getOrdersAtPrice(Price);
  void add(ClientId, OrderId, TickerId, Side, Price, Quantity);
  Quantity checkForMatch();
  Priority getNextPriority(Price);
  void addOrder(MEOrder*);
  void addOrderAtPrice(MEOrdersAtPrice*);
};
