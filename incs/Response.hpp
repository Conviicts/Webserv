#ifndef __RESPONSE_HPP__
#define __RESPONSE_HPP__

#include <iostream>
#include <map>
#include "Utils.hpp"
#include "Request.hpp"

class Response {
    private:
        Request                             &_req;
        std::string                         _method;
        std::string                         _path;
        int                                 _return_status;
        std::string                         _body;
        std::string                         _response;
        std::map<std::string, std::string>  _headers;
    public:
        Response(Request &req);
        Response(const Response &ref);
        Response& operator=(const Response& ref);
        ~Response();

		const std::string 	&response() const { return _response; };

};

#endif