#include <iostream>
#include "webserv.hpp"
#include "Utils.hpp"

int main(int ac, char **av) {
	WebServ ws = WebServ();

	try {
		if (ac != 2)
			throw Utils::Error("Main", "USAGE: ./webserv <file.conf>", -1);
		ws.parseConfigFile(av[1]);
		// std::vector<Server>::iterator it = ws.servers().begin();
		// std::cout << (*it).host() << std::endl;

    	// while (it != ws.servers().end()) {
		// 	std::cout << *it->serverNames().begin() << std::endl;
		// 	std::map<int, std::string>::const_iterator i = (*it).errorPages().begin();

		// 	while (i != (*it).errorPages().end()) {
		// 		int index = i->first;
		// 		std::string word = i->second;
		// 		std::cout << "\t" << word << " :: " << index << std::endl;
		// 		i++;
		// 	}
		// 	it++;
		// }

		ws.init_server();
		ws.wait();
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}