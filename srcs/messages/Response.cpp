#include "Response.hpp"

Response::Response(Request &req) : _req(req), _path(req.url()), _return_status(req.ret_status()) {
    _method = req.method();
    _response = "HTTP/1.1 200 OK\r\n"
"Content-type: text/html\r\n"
"\r\n"
"<html>\r\n"
"<head>\r\n"
"<title>Hello, world!</title>\r\n"
"</head>\r\n"
"<body>\r\n"
"<h1>Hello, world!</h1>\r\n"
"</body>\r\n"
"</html>\r\n\r\n";
}

Response::Response(const Response &ref) :  _req(ref._req), _path(ref._req.url()), _return_status(ref._req.ret_status()) {
    *this = ref;
}

Response& Response::operator=(const Response& ref) {
    (void)ref;
    return (*this);
}

Response::~Response() {

}