# c-http-parser

Lightweight C library for parsing simple http requests

## Example:

```c

char *req = "GET /index.html HTTP/1.1\r\nHost: "
             "localhost:8000\r\nContent-Type: text/plain\r\n\r\n";
    
http_req *parsed = http_req_parse(req);

if(parsed->method == GET){
  // Handle
}
```

