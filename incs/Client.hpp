#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__
#include "Server.hpp"
#include "Request.hpp"
#include "Utils.hpp"

class Server;

class Client {
    private:
        int             _fd;
        Server          *_server;
        Request         _request;
        std::string     _received;
        unsigned int    _bytes_count;
        bool            _is_ready;
    public:
        Client();
		Client(int fd, Server *server);
		Client(const Client &ref);
		Client& operator=(const Client &ref);
		virtual ~Client();

		int fd() const;
		int is_ready() const;

        void    receive(void);
        void    wait(void);

};

#endif