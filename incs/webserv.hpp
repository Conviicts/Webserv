#ifndef __WEBSERV_HPP__
#define __WEBSERV_HPP__

#include <vector>
#include <list>
#include <algorithm>
#include <sys/select.h>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include "Server.hpp"
#include "Client.hpp"
#include "Config.hpp"
#include "Utils.hpp"


class Server;
class Client;

class WebServ {
	private:
		std::vector<Server> _servers;
		std::vector<Client> _clients;
		
		fd_set				_fds_set;
		fd_set				_initialized_set;
		std::list<int>		_fds_list;
		int					_initialized_fd;
		int					_last_fd;

	public:
		WebServ();
		WebServ(const WebServ &);
		WebServ	&operator=(const WebServ &);
		~WebServ();
		int	parseConfigFile(char *file_path);
		int	init_server();
		int	addClient(Server &server);
		int	checkClient();
		void	removeClients(); 
		void	wait();
		std::vector<Server> &servers();
		std::vector<Client> &clients();
};

#endif