#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__

#include <vector>
#include "Server.hpp"
#include "Location.hpp"

class Server;
class Location;

class Config {
	private:
		Server 		*_server;
		Location	*_location;
	public:
		int	parseConfigFile(Server &server, std::ifstream &stream, int &count);
		int	findCharInString(std::string &s, char c);

		int	listen(std::string &content, int count);
		int	serverName(std::string &content, int count);
		int	errorPages(std::string &content, int count);
		int	rootPath(std::string &content, int count);
		int	maxBody(std::string &content, int &count);
		int	location(std::ifstream &file, std::string &content, int &count);
};

#endif