#include <cinatra/coro_http_client.hpp>

using namespace cinatra;
using namespace std::chrono_literals;

int main() {
  /*coro_http_server server(std::thread::hardware_concurrency(), 8090,
  "0.0.0.0", true); server.set_http_handler<GET>(
      "/plaintext", [](coro_http_request& req, coro_http_response& resp) {
        resp.set_delay(false);
        resp.need_date_head(false);
        resp.set_status_and_content(status_type::ok, "Hello, world!");
      });
  server.sync_start();*/
  asio::io_context ioc;

  asio::co_spawn(
      ioc,
      [&]() -> asio::awaitable<void> {
        coro_http_client client(ioc);
        auto resp = co_await client.async_get("http://www.baidu.com");
        co_return;
      },
      asio::detached);

  return ioc.run();
}
