/** ----------------------------------------------------------------------------
 * @file    hello_libhttpserver.cpp
 * @author  Sebastian Kaupper <kauppersebastian@gmail.com>
 * @brief   Example program showcasing libhttpserver
 * ---------------------------------------------------------------------------*/

#include <httpserver.hpp>

using namespace httpserver;

class hello_world_resource : public http_resource {
 public:
  const std::shared_ptr<http_response> render(const http_request&) {
    return std::shared_ptr<http_response>(new string_response("Hello, libhttpserver!"));
  }
};

int main(int argc, char** argv) {
  webserver ws = create_webserver(8080);

  hello_world_resource hwr;
  ws.register_resource("/hello", &hwr);
  ws.start(true);

  return 0;
}
