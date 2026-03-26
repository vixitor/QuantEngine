#include "../include/me_order_book.h"
MEOrderBook::MEOrderBook(TickerId ticker_id, Logger* logger, MatchingEngine* matching_engine)
    : ticker_id_(ticker_id), logger_(logger), matching_engine_(matching_engine),
      order_pool_(ME_MAX_ORDER_IDS), orders_at_price_pool_(ME_MAX_PRICE_LEVELS) {}
MEOrderBook::~MEOrderBook() {
  logger_->log("%:% %() % OrderBook\n%\n", __FILE__, __LINE__, __FUNCTION__,
               getCurrentTimeStr(&time_str_), toString(false, true));
  matching_engine_ = nullptr;
  bids_by_price_ = asks_by_price_ = nullptr;
  for (auto& itr : cid_oid_to_order_) {
    itr.fill(nullptr);
  }
}
OrderId MEOrderBook::generateNewMarketOrderId() { return next_market_order_id_++; }

Price MEOrderBook::priceToIndex(Price price) { return price % ME_MAX_PRICE_LEVELS; }

MEOrdersAtPrice* MEOrderBook::getOrdersAtPrice(Price price) {
  return price_orders_at_price_[priceToIndex(price)];
}
void MEOrderBook::add(ClientId client_id, OrderId order_id, TickerId ticker_id, Side side,
                      Price price, Quantity qty) {
  auto new_market_order_id = generateNewMarketOrderId();
  MEClientResponse client_response = {ClientResponseType::ACCEPTED,
                                      client_id,
                                      ticker_id,
                                      order_id,
                                      new_market_order_id,
                                      side,
                                      price,
                                      0,
                                      qty};
  matching_engine_->sendClientResponse(&client_response);
  auto left_quantity = checkForMatch();
  if (LIKELY(left_quantity)) {
    auto priority = getNextPriority(price);
    auto order = order_pool_.allocate(ticker_id, client_id, order_id, new_market_order_id, side,
                                      price, left_quantity, priority, nullptr, nullptr);
    addOrder(order);
    auto market_update = MEMarketUpdate{MarketUpdateType::ADD,
                                        new_market_order_id,
                                        ticker_id,
                                        side,
                                        price,
                                        left_quantity,
                                        priority};
    matching_engine_->sendMarketUpdate(&market_update);
  }
}
Priority MEOrderBook::getNextPriority(Price price) {
  auto order_at_price = getOrdersAtPrice(price);
  if (order_at_price->first_me_order_ == nullptr)
    return 1lu;
  return order_at_price->first_me_order_->prev_order_->priority_;
}

void MEOrderBook::addOrder(MEOrder* order) {
  auto orders_at_price = getOrdersAtPrice(order->price_);
  if (orders_at_price == nullptr) {
    order->prev_order_ = order->next_order_ = order;
    auto new_orders_at_price =
        orders_at_price_pool_.allocate(order->side_, order->price_, order, nullptr, nullptr);
    addOrderAtPrice(new_orders_at_price);
  } else {
    auto first_order = orders_at_price->first_me_order_;
    first_order->prev_order_->next_order_ = order;
    order->prev_order_ = first_order->prev_order_;
    order->next_order_ = first_order;
    first_order->prev_order_ = order;
  }
  cid_oid_to_order_[order->client_id_][order->client_order_id_] = order;
}

void MEOrderBook::addOrderAtPrice(MEOrdersAtPrice* orders_at_price) {
  price_orders_at_price_[priceToIndex(orders_at_price->price_)] = orders_at_price;
  auto best_orders_at_price =
      (orders_at_price->side_ == Side::BUY ? asks_by_price_ : bids_by_price_);
  if (UNLIKELY(best_orders_at_price == nullptr)) {
    (orders_at_price->side_ == Side::BUY ? asks_by_price_ : bids_by_price_) = orders_at_price;
    orders_at_price->prev_entry_ = orders_at_price->next_entry_ = orders_at_price;
  } else {
    auto target = best_orders_at_price;
    bool add_after =
        (orders_at_price->side_ == Side::BUY && orders_at_price->price_ < target->price_) ||
        (orders_at_price->side_ == Side::SELL && orders_at_price->price_ > target->price_);
    if (add_after) {
      target = target->next_entry_;
      bool add_after =
          (orders_at_price->side_ == Side::BUY && orders_at_price->price_ < target->price_) ||
          (orders_at_price->side_ == Side::SELL && orders_at_price->price_ > target->price_);
    }
    while (add_after && target != best_orders_at_price) {
      target = target->next_entry_;
      bool add_after =
          (orders_at_price->side_ == Side::BUY && orders_at_price->price_ < target->price_) ||
          (orders_at_price->side_ == Side::SELL && orders_at_price->price_ > target->price_);
    }
    if (add_after) {
      if (target == best_orders_at_price)
        target = target->prev_entry_;
      target->next_entry_->prev_entry_ = orders_at_price;
      orders_at_price->next_entry_ = target->next_entry_;
      target->next_entry_ = orders_at_price;
      orders_at_price->prev_entry_ = target;
    } else {
      target->prev_entry_->next_entry_ = orders_at_price;
      orders_at_price->prev_entry_ = target->prev_entry_;
      orders_at_price->next_entry_ = target;
      target->prev_entry_ = orders_at_price;
      if (orders_at_price->side_ == Side::BUY &&
          orders_at_price->price_ > best_orders_at_price->price_) {
        asks_by_price_ = orders_at_price;
      }
      if (orders_at_price->side_ == Side::SELL &&
          orders_at_price->price_ > best_orders_at_price->price_) {
        bids_by_price_ = orders_at_price;
      }
    }
  }
}

void MEOrderBook::cancel(ClientId client_id, OrderId order_id, TickerId ticker_id) {
  auto is_cancelable = (client_id < cid_oid_to_order_.size());
  MEOrder* exchange_order = nullptr;
  if (LIKELY(is_cancelable)) {
    auto& co_itr = cid_oid_to_order_.at(client_id);
    exchange_order = co_itr.at(order_id);
    is_cancelable = (exchange_order != nullptr);
  }
  if (UNLIKELY(!is_cancelable)) {
    client_response_ = {ClientResponseType::CANCEL_REJECTED,
                        client_id,
                        ticker_id,
                        order_id,
                        OrderId_INVALID,
                        Side::INVALID,
                        Price_INVALID,
                        Quantity_INVALID,
                        Quantity_INVALID};
  } else {
    client_response_ = {ClientResponseType::CANCELED,
                        client_id,
                        ticker_id,
                        order_id,
                        exchange_order->market_order_id_,
                        exchange_order->side_,
                        exchange_order->price_,
                        Quantity_INVALID,
                        exchange_order->quantity_};
    market_update_ = {MarketUpdateType::CANCEL, exchange_order->market_order_id_, ticker_id,
                      exchange_order->side_,    exchange_order->price_,           0,
                      exchange_order->priority_};
    removeOrder(exchange_order);
    matching_engine_->sendMarketUpdate(&market_update_);
  }
  matching_engine_->sendClientResponse(&client_response_);
}
void MEOrderBook::removeOrder(MEOrder* order) noexcept {
  auto orders_at_price = getOrdersAtPrice(order->price_);
  if (order->prev_order_ == order) {
    removeOrdersAtPrice(order->side_, order->price_);
  } else {
    const auto order_before = order->prev_order_;
    const auto order_after = order->next_order_;
    order_before->next_order_ = order_after;
    order_after->prev_order_ = order_before;
    if (orders_at_price->first_me_order_ == order) {
      orders_at_price->first_me_order_ = order_after;
    }
    order->prev_order_ = order->next_order_ = nullptr;
  }
  cid_oid_to_order_.at(order->client_id_).at(order->client_order_id_) = nullptr;
  order_pool_.deallocate(order);
}

void MEOrderBook::removeOrdersAtPrice(Side side, Price price) noexcept {
  const auto best_orders_by_price = (side == Side::BUY ? bids_by_price_ : asks_by_price_);
  auto orders_at_price = getOrdersAtPrice(price);
  if (UNLIKELY(orders_at_price->next_entry_ == orders_at_price)) {
    (side == Side::BUY ? bids_by_price_ : asks_by_price_) = nullptr;
  } else {
    orders_at_price->prev_entry_->next_entry_ = orders_at_price->next_entry_;
    orders_at_price->next_entry_->prev_entry_ = orders_at_price->prev_entry_;
    if (orders_at_price == best_orders_by_price) {
      (side == Side::BUY ? bids_by_price_ : asks_by_price_) = orders_at_price->next_entry_;
    }
    orders_at_price->prev_entry_ = orders_at_price->next_entry_ = nullptr;
  }
  price_orders_at_price_.at(priceToIndex(price)) = nullptr;
  orders_at_price_pool_.deallocate(orders_at_price);
}
