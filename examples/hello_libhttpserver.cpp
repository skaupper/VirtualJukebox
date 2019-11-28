/**
 * @file    hello_libhttpserver.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Example program showcasing libhttpserver
 *
 * @details This program starts a server on port 8080 which listens to the
 * endpoint `/hello`. To verify that it's working use `curl -XGET
 * %http://localhost:8080/hello` or use a browser and go to
 * http://localhost:8080/hello.
 */

#include <httpserver.hpp>

using namespace httpserver;

class hello_world_resource : public http_resource {
 public:
  const std::shared_ptr<http_response> render(const http_request &) {
    return std::make_shared<string_response>("Hello, libhttpserver!");
  }
};

int main(int argc, char *argv[]) {
  webserver ws = create_webserver(8080);

  hello_world_resource hwr;
  ws.register_resource("/hello", &hwr);
  ws.start(true);

  return 0;
}
