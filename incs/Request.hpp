#ifndef __REQUEST_HPP__
#define __REQUEST_HPP__

#include <iostream>
#include <map>
#include "Utils.hpp"
#include "Server.hpp"


class Request {
    private:
        std::string                         _request_content;
        int                                 _status;
        int                                 _return_status;
        std::string                         _method;
        std::string                         _url;
        std::string                         _version;
        std::string                         _language;
        std::map<std::string, std::string>  _headers;
    public:
        Request();
        Request(const Request &ref);
        Request& operator=(const Request& ref);
        ~Request();

        void    parse(std::string message, Utils::server_config config, unsigned long size);

		const std::string	&url() const { return _url; };
		const std::string	&method() const { return _method; };
		int	                ret_status() const { return _return_status; };
};

#endif