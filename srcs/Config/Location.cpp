#include <cstdlib>
#include <unistd.h>
#include "Location.hpp"
#include "webserv.hpp"
#include "Utils.hpp"

Location::Location() {
	_path = "/";
	_root = "";
	_autoindex = false;
};

Location::Location(const Location &ref) {
	*this = ref;
};

Location	&Location::operator=(const Location &ref) { 
	(void)ref;
	return (*this); 
};

Location::~Location() {};


void								Location::errorPages(std::string &data, int count) {
	std::string	path;
	std::string	error;
	int			i_error;
	size_t		pos;

	data.erase(0, 10);
	Utils::remove_whitespace(data, 0);
	if (data[0] == ';' || data.empty())
		throw Utils::Error("Location", "No content in error_page.", count);
	data.erase(data.length() - 1, 1);
	pos = data.find_first_of(" \t");
	if (pos == std::string::npos)
		return ;
	error = data.substr(0, pos);
	if (error.find_first_not_of("0123456789") != std::string::npos)
		throw Utils::Error("Location", "Invalid error page was providen.", count);
	i_error = std::atoi(error.c_str());
	data.erase(0, pos);
	Utils::remove_whitespace(data, 0);
	if (data.empty())
		throw Utils::Error("Location", "No error path was providen.", count);
	if (data[0] != '/' || data.substr(data.length() - 5, 5) != std::string(".html"))
		throw Utils::Error("Location", "The error_page must be a relative path and end with .html.", count);
	this->errorPages(i_error, data);
}
void								Location::errorPages(int id, std::string &data) { _error_pages[id] = data; }
const std::map<int, std::string>	&Location::errorPages() const { return _error_pages; }

void								Location::path(const std::string &data) { _path = data; }


const std::string 					&Location::root() const { return _root; }
void 								Location::root(std::string &data, int count) {
	char	path[512];

	data.erase(0, 4);
	Utils::remove_whitespace(data, 0);
	if (data[data.length() - 1] == ';')
		data.erase(data.length() - 1, 1);
	if (data.empty())
		throw Utils::Error("Location", "No path specified for root option.", count);
	if (!_root.empty())
		throw Utils::Error("Location", "root option already specified.", count);
	if (data[0] != '/')
		throw Utils::Error("Location", "root option must be relative path.", count);
	getcwd(path, 512);
	_root = path + data;
	data.erase();
}

const bool 							&Location::autoindex() const { return _autoindex; }
void Location::autoindex(std::string &data, int count) {
	std::string	option;
	data.erase(0, 9);
	Utils::remove_whitespace(data, 0);
	if (data.empty() || data[0] == ';')
		throw Utils::Error("Location", "No value specified for autoindex.", count);

	option = data.substr(0, data.length() - 1);
	if (option == std::string("on"))
		_autoindex = true;
	else if (option == std::string("off"))
		_autoindex = false;
	else
		throw Utils::Error("Location", "Unknow option in autoindex.", count);
	data.erase();
}

