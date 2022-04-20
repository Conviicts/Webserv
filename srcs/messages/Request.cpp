#include "Request.hpp"

Request::Request() : _status(0) {
    
}

Request::Request(const Request &ref) {
    (void)ref;
}

Request& Request::operator=(const Request& ref) {
    (void)ref;
    return (*this);
}

Request::~Request() {

}

void    Request::parse(std::string message, Utils::server_config config, unsigned long size) {
    (void)size;
    std::string     line;
    std::string     key;
    std::string     data;

    if (_status == 0) {
        line = Utils::cut_message(message, true, 0);
        _method = line.substr(0, line.find(" "));
        line.erase(0, _method.size() + 1);
        _url = line.substr(0, line.find(" "));
        line.erase(0, _url.size() + 1);
        _version = line.substr(0, line.find(" "));
        line = Utils::cut_message(message, true, 0);
        _status = 1;
    }
    while (_status == 1 && line.find(":") != std::string::npos) {
        key = line.substr(0, line.find(":"));
        data = line.substr(line.find(":") + 2);
        if (key.size() > 0 && data.size() > 0) {
            _headers[key] = data;
            line = Utils::cut_message(message, true, 0);
        }
        _status = 2;
        if (_headers.find("Accept-Language") != _headers.end()) {
            if (_headers["Accept-Language"].find(",") == std::string::npos)
                _language = "en_US";
            else {
                while (_headers["Accept-Language"].find(",") != std::string::npos) {
                    _language += _headers["Accept-Language"].substr(0, _headers["Accept-Language"].find(",") + 1);
                    if (_language.find(";") != std::string::npos)
                        _language.erase(_language.find(";"), _language.length());
                    _headers["Accept-Language"].erase(0, _headers["Accept-Language"].find(",") + 1);
                }
                _language += _headers["Accept-Language"];
                if (_language.find(";") != std::string::npos)
                    _language.erase(_language.find(";"), _language.length());
            }
        }
    }
    _return_status = 200;
    if (_method.length() == 0 || (_method != "GET" && _method != "POST"
        && _method != "PUT" && _method != "DELETE"))
        _return_status = 400;
    else if (_method != "GET" && _method != "POST" && _method != "DELETE")
        _return_status = 501;
    if (_url.length() == 0 || _url[0] != '/')
        _return_status = 400;
    if (_version.length() == 0 || (_version != "HTTP/1.1" && _version != "HTTP/1.1\r"))
        _return_status = 400;
    if (_headers.find("Host") == _headers.end())
		_return_status = 400;
    //check methods is in config
    if (_method == "POST" && _status == 2 && _return_status < 400){}
		//parse body
    else
        _status = 3;
}
