#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <exception>
#include <iostream>
#include <map>
#include <vector>
#include "Location.hpp"

class Location;

class Utils {
    public:
        class Error : public std::exception {
            private:
                std::string	_message;
            public:
                Error(std::string className , std::string message, int line) {
                    if (line != -1)
                        _message = className + ":" + itoa(line) + " > " + message;
                    else
                        _message = "Error: " + message;
                }
                ~Error() throw() {};
                virtual const char *what() const throw() {
                    return (_message.c_str());
                };
        };
        static std::string  itoa(int a);
        static std::string	cut_message(std::string &s, bool remove_endline, int end);
        static void         remove_whitespace(std::string &s, int i);
        struct server_config {
            std::vector<std::string>		server_names;
            std::string						host;
            std::string						root;
            unsigned int					port;
            unsigned int					fd;
            unsigned int					client_max_body_size;
            std::map<int, std::string>		error_pages;
            std::vector<Location>			locations;
        };
};

#endif