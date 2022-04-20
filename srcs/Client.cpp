#include "Client.hpp"
#include <sys/socket.h>

Client::Client() { }

Client::Client(int fd, Server *server) : _fd(fd), _server(server) {
    _bytes_count = 0;
    _is_ready = false;
}

Client::Client(const Client &ref) : _fd(ref._fd), _server(ref._server) {
    *this = ref;
}

Client &Client::operator=(const Client &ref) {
    _fd = ref._fd;
    _server = ref._server;

return (*this);
}

Client::~Client() {
}

int Client::fd() const { return _fd; }
int Client::is_ready() const { return _is_ready; }

void    Client::receive(void) {
    char            buffer[4096] = {0};
    int             ret;

    while (1) {
        ret = recv(_fd, buffer, 4096, 0);
        if (ret == -1)
            exit(1);
        buffer[ret] = 0;
        _received += buffer;
        _bytes_count += ret;
        if (ret == 0)
            break ;
        
        break;
    }
    _is_ready = true;
}

void    Client::wait(void) {
    
}
