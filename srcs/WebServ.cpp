#include "webserv.hpp"

WebServ::WebServ() {};
WebServ::WebServ(const WebServ &) {};
WebServ	&WebServ::operator=(const WebServ &) { return (*this); };
WebServ::~WebServ() {};

int	WebServ::parseConfigFile(char *file_path) {
	std::ifstream	file(file_path);
	std::string		f_path(file_path);
	std::string		content;
	Config			config;
	Server			server;
	int				count = 0;

	if (f_path.size() <= 5 || f_path.substr(f_path.size() - 5, f_path.size()) != ".conf")
		throw Utils::Error("Main", "Bad config file extension.", count);
	if (!file.is_open())
		throw Utils::Error("Main", "Unable to open config file.", count);
	count = 0;
	while (std::getline(file, content)) {
		Utils::remove_whitespace(content, 0);
		if (content.empty() || content[0] == '#') {
			count++;
			continue;
		}
		count++;
		
		if (!content.compare(0, 5, "server"))
			throw Utils::Error("Main", "wrong config parameter.", count);
		Utils::remove_whitespace(content, 6);
		if (!content.compare(0, 6, "server{"))
			throw Utils::Error("Main", "wrong config parameter.", count);
		config.parseConfigFile(server, file, count);
		_servers.push_back(server);
	}
	if (_servers.size() == 0)
		throw Utils::Error("Main", "No server found.", count);
	return (1);
}

int WebServ::init_server() {
	struct sockaddr_in	address;
	int					optval;
	int					servers = 0;
	std::vector<Server>::iterator server = _servers.begin();
	
	memset(&address, 0, sizeof(address));
	FD_ZERO(&_fds_set);
	address.sin_family = AF_INET;
	if (_servers.size() <= 0)
		return (0);
	while (server != _servers.end())
	{
		int	fd;

		fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
			break;
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
			close(fd);
			break ;
		}
		address.sin_addr.s_addr = inet_addr((server)->host().c_str());
		address.sin_port = htons((server)->port());
		if (bind(fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
			_servers.erase(server);
			close(fd);
			break ;
		}
		if (listen(fd, SOMAXCONN) < 0) {
			_servers.erase(server);
			close(fd);
			break ;
		}
		(server)->fd(fd);
		FD_SET(fd, &_fds_set);
		_fds_list.push_back(fd);
		_last_fd = fd;
		server++;
		servers++;
	}
	if (!servers)
		return (0);
	_last_fd = *std::max_element(_fds_list.begin(), _fds_list.end());
	return (1);
}


int		WebServ::addClient(Server &server) {
	Client	client;
	int		fd;

	_initialized_fd--;
	if (servers().size() + clients().size() < FD_SETSIZE) {
		if ((fd = accept(server.fd(), 0, 0)) == -1)
			return (-1);
		std::cout << "New client connection accepted. fd: [" << fd << "]" << std::endl;
		FD_SET(fd, &_fds_set);
		_fds_list.push_back(fd);
		_last_fd = *std::max_element(_fds_list.begin(), _fds_list.end());
		Client nClient(fd, &server);
		clients().push_back(nClient);
	} else {
		return (0);
	}
	return (1);
}

int		WebServ::checkClient() {
	int	fd;

	std::vector<Client>::iterator client = clients().begin();
	
	while (client != clients().end() && _initialized_fd != 0) {
		fd = (client)->fd();
		if (FD_ISSET(fd, &_initialized_set)) {
			_initialized_fd--;
			client->receive();
			if (client->is_ready()) {
				client->wait();
				std::cout << "Client: [" << fd << "] disconnected." << std::endl;
				close(fd);
				FD_CLR(fd, &_fds_set);
				_fds_list.remove(fd);
				_last_fd = *std::max_element(_fds_list.begin(), _fds_list.end());
				client = clients().erase(client);
			}
		}
		client++;
	}
	return (1);
}

void	WebServ::removeClients() {
	for (std::vector<Client>::iterator client = clients().begin();
			client != clients().end() && _initialized_fd != 0 ; client++)
		(*client).~Client();
	clients().clear();

}

void	WebServ::wait() {
	std::cout << "Waiting for connections." << std::endl;
	while (1) {
		std::vector<Server>::iterator server = _servers.begin();
		_initialized_set = _fds_set;
		_initialized_fd = select(_last_fd + 1, &_initialized_set, 0, 0, 0);
		while (server != _servers.end()) {
			if (FD_ISSET((server)->fd(), &_initialized_set))
				addClient(*server);
			server++;
		}
		checkClient();
	}
	removeClients();
}

std::vector<Server> &WebServ::servers() { return _servers; };
std::vector<Client> &WebServ::clients() { return _clients; };