#include "http-parser.h"

char *req1 = "GET /index.html HTTP/1.1\r\nHost: "
             "localhost:8000\r\nContent-Type: text/plain\r\n\r\n";
char * req2 = "POST /test HTTP/1.1\r\nHost: foo.example\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nfield1=value1&field2=value2";

int main(int argc, char *argv[]) {
  printf("HTTP Request parser\n");


  printf("GET test: %d\n", string_to_http_method("GET", 3));
  printf("POST test: %d\n", string_to_http_method("POST", 4));
  
  printf("----- 1 -----\n");
  http_req *parsed = http_req_parse(req1);
  printf("Method: %d, Target: %s, Version: %d.%d\n", parsed->method, parsed->target,
         parsed->version->version_major, parsed->version->version_minor);
  
  http_header * hdr = parsed->headers;
  while (hdr != NULL) {
    printf("Header Key: %s\tHeader Val: %s\n", hdr->key, hdr->value);
    hdr = hdr->next;
  }
  
  // printf("Body: (%d) %s", strlen(parsed->body), parsed->body);
  
  printf("----- 2 -----\n");
  parsed = http_req_parse(req2);

  printf("Method: %d, Target: %s, Version: %d.%d\n", parsed->method, parsed->target,
         parsed->version->version_major, parsed->version->version_minor);
  
  hdr = parsed->headers;
  while (hdr != NULL) {
    printf("Header Key: %s\tHeader Val: %s\n", hdr->key, hdr->value);
    hdr = hdr->next;
  }

  printf("Body: (%d) %s\n", strlen(parsed->body), parsed->body);

  return 0;
}
