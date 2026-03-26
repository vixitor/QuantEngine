#include "../include/matching_engine.h"
#include "../include/me_order_book.h"
#include <thread>

MatchingEngine::MatchingEngine(ClientRequestLFQueue* client_request,
                               ClientResponseLFQueue* client_response,
                               MEMarketUpdateLFQueue* market_update)
    : incoming_request_(client_request), outgoing_response_(client_response),
      outgoing_md_updates_(market_update), logger_("exchange_matching_enging.log") {
        for(int i = 0;i < ticker_order_book_.size();i++){
    ticker_order_book_[i] = new MEOrderBook(i, &logger_, this);
  }
      }
MatchingEngine::~MatchingEngine() {
  run_ = false;
  using namespace std::literals::chrono_literals;
  std::this_thread::sleep_for(1s);
  incoming_request_ = nullptr;
  outgoing_response_ = nullptr;
  outgoing_md_updates_ = nullptr;
  for(int i = 0;i < ticker_order_book_.size();i++){
    delete ticker_order_book_[i];
    ticker_order_book_[i] = nullptr;
  }
}
void MatchingEngine::start() {
  run_ = true;
  ASSERT(createAndStartThread(-1, "MatchingEngine", [this]() { run(); }) != nullptr,
         "Failed to start MatchingEngine thread.");
}
void MatchingEngine::stop() { run_ = false; }
void MatchingEngine::run() {
  logger_.log("%:% %() %\n", __FILE__, __LINE__, __FUNCTION__, getCurrentTimeStr(&time_str_));
  while (run_) {
    const auto me_client_request = incoming_request_->getNextToRead();
    if (me_client_request != nullptr) {
      logger_.log("%:% %() % Processing %\n", __FILE__, __LINE__, __FUNCTION__,
                  getCurrentTimeStr(&time_str_), me_client_request->toString());
      processClientRequest(me_client_request);
      incoming_request_->updataReadIndex();
    }
  }
}
void MatchingEngine::processClientRequest(const MEClientRequest* client_request) {
  auto order_book = ticker_order_book_[client_request->ticker_id_];
  switch (client_request->type_) {
  case ClientRequestType::NEW:
    break;
  case ClientRequestType::CANCEL:
    break;
  default:
    FATAL("Received invalid client-request-type:" +
          clientRequestTypeToString(client_request->type_));
    break;
  }
}

void MatchingEngine::sendClientResponse(const MEClientResponse* client_response) {
  logger_.log("%:% %() % Sending %\n", __FILE__, __LINE__, __FUNCTION__,
              getCurrentTimeStr(&time_str_), client_response->toString());
  auto next = outgoing_response_->getNextToWriteTo();
  *next = *client_response;
  outgoing_response_->updateWriteIndex();
}

void MatchingEngine::sendMarketUpdate(const MEMarketUpdate* market_update) noexcept {
  logger_.log("%:% %() % Sending %\n", __FILE__, __LINE__, __FUNCTION__,
              getCurrentTimeStr(&time_str_), market_update->toString());
  auto next_write = outgoing_md_updates_->getNextToWriteTo();
  *next_write = *market_update;
  outgoing_md_updates_->updateWriteIndex();
}