#pragma once
#include "client_request.h"
#include "client_response.h"
#include "lf_queue.h"
#include "logger.h"
#include "macros.h"
#include "market_update.h"
#include "me_order.h"
#include "me_order_book.h"
#include "thread_utils.h"
class MEOrderBook;
typedef std::array<MEOrderBook*, ME_MAX_TICKERS> OrderBookHashMap;
class MatchingEngine {
public:
  MatchingEngine(ClientRequestLFQueue* client_request, ClientResponseLFQueue* client_response,
                 MEMarketUpdateLFQueue* market_update);
  ~MatchingEngine();
  MatchingEngine() = delete;
  MatchingEngine(const MatchingEngine&) = delete;
  MatchingEngine(const MatchingEngine&&) = delete;
  MatchingEngine& operator=(const MatchingEngine&) = delete;
  MatchingEngine& operator=(const MatchingEngine&&) = delete;
  void start();
  void stop();
  void run();
  void processClientRequest(const MEClientRequest*);
  void sendClientResponse(const MEClientResponse*);
  void sendMarketUpdate(const MEMarketUpdate*) noexcept;

private:
  OrderBookHashMap ticker_order_book_;
  ClientRequestLFQueue* incoming_request_ = nullptr;
  ClientResponseLFQueue* outgoing_response_ = nullptr;
  MEMarketUpdateLFQueue* outgoing_md_updates_ = nullptr;
  volatile bool run_ = false;
  std::string time_str_;
  Logger logger_;
};