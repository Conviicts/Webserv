#include <vector>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include "Config.hpp"
#include "Location.hpp"
#include "webserv.hpp"
#include "Utils.hpp"

int	Config::findCharInString(std::string &s, char c) {
	int	count;

	count = 0;
	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] == c)
			count++;
	}
	return (count);
}

int	Config::parseConfigFile(Server &server, std::ifstream &stream, int &count) {
	std::string	content;
	int			i;

	i = 0;
	_server = &server;
	while (std::getline(stream, content)) {
		Utils::remove_whitespace(content, 0);
		if (content.empty() || content[0] == '#') {
			count++;
			continue;
		}
		count++;
		if (content[content.length() - 1] != ';' && content[0] != '}' && content.compare(0, 8, "location"))
			throw Utils::Error("Config", "you need to add semicolon at the end of the line.", count);
		if (findCharInString(content, ';') > 1)
			throw Utils::Error("Config", "Too many semicolon in line.", count);
		if (content.substr(0, content.find_first_of(" \t")) == "listen")
			listen(content, count);
		else if (content.substr(0, content.find_first_of(" \t")) == "server_name")
			serverName(content, count);
		else if (content.substr(0, content.find_first_of(" \t")) == "error_page")
			errorPages(content, count);
		else if (content.substr(0, content.find_first_of(" \t")) == "root")
			rootPath(content, count);
		else if (content.substr(0, content.find_first_of(" \t")) == "location")
			location(stream, content, count);
		else if (content.substr(0, content.find_first_of(" \t")) == "client_max_body_size")
			maxBody(content, count);
		else if (content[0] == '}') {
			i = 1;
			break ;
		}
		else
			throw Utils::Error("Config", "Unknown parameter in server block", count);
	}
	if (!i)
		throw Utils::Error("Config", "Missing closing bracket.", count);
	return (1);
}

int	Config::serverName(std::string &content, int count) {
	std::string	name;
	size_t		pos;
	
	content.erase(0, 11);
	Utils::remove_whitespace(content, 0);
	if (content[0] == ';' || content.empty())
		throw Utils::Error("Config", "No content in server_name.", count);
	content.erase(content.length() - 1, 1);
	pos = content.find_first_of(" \t");
	while (pos != std::string::npos) {
		_server->serverNames(content.substr(0, pos));
		content.erase(0, pos);
		Utils::remove_whitespace(content, 0);
		pos = content.find_first_of(" \t");
	}
	_server->serverNames(content);
	return (1);
}

int Config::errorPages(std::string &content, int count) {
	std::string	path;
	std::string	error;
	int			i_error;
	size_t		pos;

	content.erase(0, 10);
	Utils::remove_whitespace(content, 0);
	if (content[0] == ';' || content.empty())
		throw Utils::Error("Config", "No content in error_page.", count);
	content.erase(content.length() - 1, 1);
	pos = content.find_first_of(" \t");
	if (pos == std::string::npos)
		return (0);
	error = content.substr(0, pos);
	if (error.find_first_not_of("0123456789") != std::string::npos)
		throw Utils::Error("Config", "Invalid error page was providen.", count);
	i_error = std::atoi(error.c_str());
	content.erase(0, pos);
	Utils::remove_whitespace(content, 0);
	if (content.empty())
		throw Utils::Error("Config", "No error path was providen.", count);
	if (content[0] != '/' || content.substr(content.length() - 5, 5) != std::string(".html"))
		throw Utils::Error("Config", "The error_page must be a relative path and end with .html.", count);
	_server->errorPages(i_error, content);
	return (1);
}

int	Config::listen(std::string &content, int count) {
	size_t			pos;
	unsigned int	port;
	
	content.erase(0, 6);
	Utils::remove_whitespace(content, 0);
	if (content[0] == ';' || content.empty())
		throw Utils::Error("Config", "No content in listen.", count);
	content.erase(content.length() - 1, 1);
	pos = content.find(":");
	if (pos != std::string::npos) {
		port = std::atoi(content.substr(pos + 1, content.length()).c_str());
		if (port < 1 || port > 65535)
			throw Utils::Error("Config", "Invalid port for listen.", count);
		_server->host(content.substr(0, pos));
		_server->port(port);
	}
	else
		_server->host(content);
	return (1);
}

int	Config::rootPath(std::string &content, int count) {
	char	tmp[512];

	content.erase(0, 4);
	Utils::remove_whitespace(content, 0);
	if (content[0] == ';' || content.empty())
		throw Utils::Error("Config", "No content in listen.", count);
	content.erase(content.length() - 1, 1);
	if (!_server->root().empty())
		throw Utils::Error("Config", "root path already set.", count);
	getcwd(tmp, 512);
	_server->root(tmp + content);
	return (1);
}

int	Config::maxBody(std::string &content, int &count) {
	content.erase(0, 20);
	Utils::remove_whitespace(content, 0);
	if (content[0] == ';' || content.empty())
		throw Utils::Error("Config", "No content in client_max_body_size.", count);
	content.erase(content.length() - 1, 1);
	if (content.find_first_not_of("0123456789KkMmGg") != std::string::npos)
		throw Utils::Error("Config", "Invalid value in client_max_body_size.", count);
	if (content[content.length() - 1] == 'K' || content[content.length() - 1] == 'k') {
		content.erase(content.length() - 1, 1);
		_server->max_body(std::atol(content.c_str()) * 1000);
	}
	else if (content[content.length() - 1] == 'M' || content[content.length() - 1] == 'm') {
		content.erase(content.length() - 1, 1);
		_server->max_body(std::atol(content.c_str()) * 100000);
	}
	else if (content[content.length() - 1] == 'G' || content[content.length() - 1] == 'g') {
		content.erase(content.length() - 1, 1);
		_server->max_body(std::atol(content.c_str()) * 1000000000);
	}
	std::cout << _server->max_body() << std::endl;
	return (1);
}

int	Config::location(std::ifstream &file, std::string &content, int &count) {
	Location 	loc;
	std::string	path;
	size_t		pos;

	content.erase(0, 8);
	if (content.empty())
		throw Utils::Error("Config", "empty location path.", count);
	Utils::remove_whitespace(content, 0);
	if (content[0] != '/')
		throw Utils::Error("Config", "The location path must only be absolute", count);
	pos = content.find_first_of(" \t");
	if (pos == std::string::npos)
		throw Utils::Error("Config", "Missing bracket on location.", count);
	path = content.substr(0, pos);
	//check duplicates of path
	loc.path(path);
	content.erase(0, pos);
	Utils::remove_whitespace(content, 0);
	if (content.empty() || content[0] != '{')
		return (0);
	_location = &loc;
	while (std::getline(file, content)) {
		std::string str;
		Utils::remove_whitespace(content, 0);
		if (content.empty() || content[0] == '#') {
			count++;
			continue ;
		}
		count++;
		if (content[content.length() - 1] != ';' && content[0] != '}')
			throw Utils::Error("Config", "Missing semicolon at end of line in location.", count);
		if (findCharInString(content, ';') > 1)
			throw Utils::Error("Config", "Too many semicolon in location parameter.", count);
		str = content.substr(0, content.find_first_of(" \t"));
		if (str == "root")
			_location->root(content, count);
		else if (str == "autoindex")
			_location->autoindex(content, count);
		else if (str == "error_page")
			_location->errorPages(content, count);
		else if (content[0] == '}') {
			_server->location(loc);
			return (1);
		}
		else
			throw Utils::Error("Config", "Unknown parameter in location block.", count);
	}

	return (1);
}
