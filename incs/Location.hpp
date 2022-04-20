#ifndef __LOCATION_HPP__
#define __LOCATION_HPP__

#include <map>
#include <vector>
#include <iostream>
#include "webserv.hpp"

class Location {
	private:
		std::map<int, std::string>		_error_pages;
		std::string						_path;
		std::string						_root;

		bool							_autoindex;

	public:
		Location();
		Location(const Location &ref);
		Location	&operator=(const Location &ref);
		~Location();
		
		void							errorPages(std::string &data, int count);
		void							errorPages(int id, std::string &data);
		const std::map<int, std::string>&errorPages() const;
		
		void							path(const std::string &data);
		const							std::string &path();
		
		void							root(std::string &data, int count);
		const							std::string &root() const;

		void							autoindex(std::string &data, int count);
		const							bool &autoindex() const;

};

#endif