#include "Server.hpp"

Server::Server() {
    _host = "127.0.0.1";
    _port = 80;
};

Server::Server(const Server &ref) {
    *this = ref;
};
Server &Server::operator=(const Server &ref) {
    if (this != &ref) {
        _error_pages = ref._error_pages;
        _server_names = ref._server_names;
        _host = ref._host;
        _root = ref._root;
        _port = ref._port;
        _fd = ref._fd;
    }
    return (*this);
};

Server::~Server() {};

Utils::server_config	Server::serverConfig() {
    Utils::server_config   config;

    config.server_names = _server_names;
    config.host = _host;
    config.root = _root;
    config.port = _port;
    config.fd = _fd;
    config.client_max_body_size = _client_max_body_size;
    config.error_pages = _error_pages;
    config.locations = _locations;
    return (config);
}

void	Server::sendResponse(int fd, Request &req) {
    Response    res(req);
    std::string data;

    std::cout << res.response() << std::endl;
	send(fd, res.response().c_str(), res.response().length(), 0);
}


void							    Server::errorPages(int id, std::string &data) { _error_pages[id] = data; }
const std::map<int, std::string>    &Server::errorPages() const { return _error_pages; }

void							    Server::serverNames(const std::string &data) { _server_names.push_back(data); }
const std::vector<std::string>      &Server::serverNames() const { return _server_names; }

void							    Server::location(const Location &data) { _locations.push_back(data); }
const std::vector<Location>         &Server::location() const { return _locations; }

void							    Server::host(const std::string &data) { _host = data; }
const std::string                   &Server::host() const { return _host; }

void							    Server::root(const std::string &data) { _root = data; }
const std::string                   &Server::root() const { return _root; }

void							    Server::port(const unsigned int &data) { _port = data; }
const unsigned int                  &Server::port() const { return _port; }

void								Server::max_body(const unsigned long &data) { _client_max_body_size = data; }
const unsigned long					&Server::max_body() const { return _client_max_body_size; }

void							    Server::fd(const unsigned int &data) { _fd = data; }
const int                           &Server::fd() const { return _fd; }