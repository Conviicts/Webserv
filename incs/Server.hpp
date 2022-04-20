#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <map>
#include <vector>
#include <iostream>
#include "Location.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Utils.hpp"

class Location;

class Server {
	private:
		std::map<int, std::string>		_error_pages;
		std::vector<std::string>		_server_names;
		std::vector<Location>			_locations;
		std::string						_host;
		std::string						_root;
		unsigned int					_port;
		unsigned long					_client_max_body_size;
		int								_fd;

	public:
		Server();
		Server(const Server &ref);
		Server &operator=(const Server &ref);
		~Server();

		void	sendResponse(int fd, Request &req);
		
		Utils::server_config	serverConfig();
		
		void								errorPages(int id, std::string &data);
		const std::map<int, std::string>	&errorPages() const;
		
		void								serverNames(const std::string &data);
		const std::vector<std::string>		&serverNames() const;

		void								location(const Location &data);
		const std::vector<Location>			&location() const;

		void								host(const std::string &data);
		const std::string					&host() const;
		
		void								root(const std::string &data);
		const std::string 					&root() const;

		void								port(const unsigned int &data);
		const unsigned int					&port() const;

		void								max_body(const unsigned long &data);
		const unsigned long					&max_body() const;

		void								fd(const unsigned int &data);
		const int							&fd() const;
};

#endif