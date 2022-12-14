#include "http-parser.h"

char *req1 = "GET /index.html HTTP/1.1\r\nHost: "
             "localhost:8000\r\nContent-Type: text/plain\r\n\r\n";

int main(int argc, char *argv[]) {
  printf("HTTP Request parser\n");

  http_req *parsed = http_req_parse(req1);

  printf("GET test: %d\n", string_to_http_method("GET", 3));
  printf("POST test: %d\n", string_to_http_method("POST", 4));
  printf("Method: %d, Target: %s, Version: %d.%d\n", parsed->method, parsed->target,
         parsed->version->version_major, parsed->version->version_minor);
  return 0;
}
