#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TARGET_BUFFER_SIZE 64
#define HTTP_HEADER_KEY_SIZE 16
#define HTTP_HEADER_VALUE_SIZE 64
#define HTTP_HEADER_LINE_LEN 128

// HTTP method
enum HTTP_METHOD {
  GET,
  HEAD,
  POST,
  PUT,
  DELETE,
  PATCH,
  OPTIONS,
  CONNECT,
  TRACE,
  INVALID,
  RESERVED
};

// Represent the "1.1" in eg HTTP/1.1
typedef struct http_version {
  unsigned char version_major;
  unsigned char version_minor;
} http_version;

// Represent a header line eg "Host: localhost:8000"
// Headers come line by line so key shoud contain text up to ":"
// value contains the rest of the string up to the newline
typedef struct http_header {
  char key[HTTP_HEADER_KEY_SIZE];
  char value[HTTP_HEADER_VALUE_SIZE];
  struct http_header *next;
} http_header;

// Structure containing the whole request
typedef struct http_req {
  enum HTTP_METHOD method;
  http_version * version;
  char target[TARGET_BUFFER_SIZE];
  http_header * headers;
  char * body;
} http_req;


enum HTTP_METHOD string_to_http_method(char *string_start, unsigned char len);

http_version * string_to_http_version(char*string_start, unsigned char len);

http_req *http_req_parse(char *raw_req);

// Free request and headers
void free_http_req(void);

#endif
