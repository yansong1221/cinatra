#pragma once

#include <asio/steady_timer.hpp>
#include <atomic>
#include <chrono>
#include <string>

#include "session.hpp"
#include "ylt/coro_io/coro_io.hpp"

namespace cinatra {

class session_manager {
 public:
  static session_manager &get() {
    static session_manager instance;
    return instance;
  }

  std::string generate_session_id() {
    auto tp = std::chrono::high_resolution_clock::now();
    auto nano = tp.time_since_epoch().count();
    id_++;
    return std::to_string(nano).append(std::to_string(id_));
  }

  std::shared_ptr<session> get_session(const std::string &session_id) {
    std::unique_lock<std::mutex> lock(mtx_);

    std::shared_ptr<session> new_session = nullptr;
    auto iter = map_.find(session_id);
    if (iter != map_.end()) {
      return iter->second;
    }
    else {
      new_session =
          std::make_shared<session>(session_id, session_timeout_, true);
      map_.insert({session_id, new_session});
    }

    return new_session;
  }

  void remove_expire_session() {
    std::unique_lock<std::mutex> lock(mtx_);

    auto now = std::time(nullptr);
    for (auto it = map_.begin(); it != map_.end();) {
      if (it->second->get_time_stamp() <= now)
        it = map_.erase(it);
      else
        ++it;
    }
  }

  bool check_session_existence(const std::string &session_id) {
    std::unique_lock<std::mutex> lock(mtx_);

    return map_.find(session_id) != map_.end();
  }

 private:
  session_manager(){

  };
  session_manager(const session_manager &) = delete;
  session_manager(session_manager &&) = delete;

  std::atomic_int64_t id_ = 0;
  std::unordered_map<std::string, std::shared_ptr<session>> map_;
  std::mutex mtx_;

  // session_timeout_ should be no less than 0
  std::size_t session_timeout_ = 86400;
};

}  // namespace cinatra