/*
 * Copyright (c) 2023, Alibaba Group Holding Limited;
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#if defined(YLT_ENABLE_SSL) || defined(CINATRA_ENABLE_SSL)
#include <asio/ssl.hpp>
#endif

#include <asio/connect.hpp>
#include <asio/experimental/channel.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/read_at.hpp>
#include <asio/read_until.hpp>
#include <asio/write.hpp>
#include <asio/write_at.hpp>
#include <chrono>
#include <deque>
#include "../util/type_traits.h"

namespace coro_io {
template <typename T>
constexpr inline bool is_lazy_v =
    util::is_specialization_v<std::remove_cvref_t<T>, asio::awaitable>;

template <typename Duration>
inline asio::awaitable<std::error_code> sleep_for(Duration d) {
  asio::steady_timer timer(co_await asio::this_coro::executor);
  timer.expires_after(d);
  std::error_code ec;
  co_await timer.async_wait(asio::redirect_error(asio::use_awaitable, ec));
  co_return ec;
}

}  // namespace coro_io
