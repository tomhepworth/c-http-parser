#include "http-parser.h"

// Read len chars from string_start and convert to HTTP_METHOD enum
enum HTTP_METHOD string_to_http_method(char *string_start, unsigned char len) {

  // Check string start + len wont overflow
  if (strlen(string_start) < len)
    return INVALID;

  enum HTTP_METHOD res = INVALID;

  char substr[len];
  memcpy(substr, string_start, len);

  // Horrible if ladder
  // placed in vague order of use-frequency to save comparison
  if (!strncmp("GET", substr, len))
    res = GET;
 
  else if (!strncmp("POST", substr, len))
    res = POST;

  else if (!strncmp("DELETE", substr, len))
    res = DELETE;

  else if (!strncmp("HEAD", substr, len))
    res = HEAD;

  else if (!strncmp("PUT", substr, len))
    res = PUT;

  else if (!strncmp("PATCH", substr, len))
    res = PATCH;

  else if (!strncmp("OPTIONS", substr, len))
    res = OPTIONS;

  else if (!strncmp("CONNECT", substr, len))
    res = CONNECT;

  else if (!strncmp("TRACE", substr, len))
    res = TRACE;

  return res;
}


http_version * string_to_http_version(char*string_start, unsigned char len)
{
  // Check string start + len wont overflow
  if (strlen(string_start) < len)
    return 0; //Return null
  
  //String will be in format HTTP/%d.%d
  
  http_version * res = calloc(1, sizeof(http_version));
  
  //First characters should always be "HTTP/"
  char * preamble = "HTTP/";
  

  // Note char - '0' converts to int
  // Next single char will be the version version_major 
  res->version_major = string_start[strlen(preamble)] - '0';
 
  // version_minor will follow a '.'
  res->version_minor = string_start[strlen(preamble) + 2] - '0';
  
  return res;
}

http_req *http_req_parse(char *raw_req) {
  printf("Test\n");

  http_req *res = calloc(1, sizeof(http_req));

  // Initial line is in format:
  // METHOD TARGET HTTP/version_major.version_minor\r\n
  // So can be separated by space
  
  // Method is first string
  char method[16];
  sscanf(raw_req, "%s", method);
  res->method = string_to_http_method(method, strlen(method));

  // Target is second string
  sscanf(raw_req, "%*s %s ", res->target);

  //Http version string is third string
  char version[16];
  sscanf(raw_req, "%*s %*s %s", version);
  res->version = string_to_http_version(version, strlen(version));

  return res;
}
