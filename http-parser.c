#include "http-parser.h"
#include <stdbool.h>
#include <string.h>

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

http_version *string_to_http_version(char *string_start, unsigned char len) {
  // Check string start + len wont overflow
  if (strlen(string_start) < len)
    return 0; // Return null

  // String will be in format HTTP/%d.%d

  http_version *res = calloc(1, sizeof(http_version));

  // First characters should always be "HTTP/"
  char *preamble = "HTTP/";

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
  res->headers = NULL;

  unsigned char line_count = 0;
  bool in_header_section = true;

  char *delim = "\r\n";
  char *token = raw_req;

  while (1) {
    char *line_end = strstr(token, delim);
    if (line_end == NULL)
      break;

    // Get line as a single string
    char line_buf[HTTP_HEADER_LINE_LEN];
    size_t line_len = line_end - token;
    memcpy(line_buf, token, line_len);
    line_buf[line_len] = '\0';

    printf("tok: %s\n", line_buf);

    // If line is blank, header section is over
    if (!strcmp(line_buf, "")) {
      in_header_section = false;
    }

    // Parse first line
    if (line_count == 0) {
      // Initial line is in format:
      // METHOD TARGET HTTP/version_major.version_minor\r\n
      // So can be separated by space

      // Method is first string
      char method[16];
      sscanf(raw_req, "%s", method);
      res->method = string_to_http_method(method, strlen(method));

      // Target is second string
      sscanf(raw_req, "%*s %s ", res->target);

      // Http version string is third string
      char version[16];
      sscanf(raw_req, "%*s %*s %s", version);
      res->version = string_to_http_version(version, strlen(version));

    } else if (in_header_section) {
      // If not first line, but not seen blank line, parse headers

      http_header *header = calloc(1, sizeof(http_header));
      header->next = NULL;

      // Find the prev_header and set up linked list
      http_header *prev_header;

      // If not first insertion, find end of linked list
      if (res->headers != NULL) {
        prev_header = res->headers;

        while (prev_header->next != NULL)
          prev_header = prev_header->next;

        prev_header->next = header;
      } else {
        // First insertion
        res->headers = header;
      }

      // Format is "KEY: VALUE"
      // Delimit on ':'
      char *split_point = strchr(line_buf, ':');
      size_t key_len = split_point - line_buf;
      memcpy(header->key, line_buf, key_len);

      // skip over ": " and copy to end of string
      split_point += 2;
      size_t val_len = strlen(line_buf) - strlen(header->key) - 2;
      memcpy(header->value, split_point, val_len);

    } else { // Parse content
      // Skip over blank line
      token += strlen(delim);
      printf("Body (%d): %s", strlen(token), token);

      if (strlen(token) <= 0) {
        res->body = NULL;
      } else {
        // Copy into result body, adding space for null char
        res->body = (char *)calloc(1, (1 + strlen(token)) * sizeof(char));
	memcpy(res->body, token, strlen(token));
      }
    }

    token = line_end + strlen(delim); // Skip past the delimter itself
    line_count++;
  }
  printf("Done parsing\n");
  return res;
}
