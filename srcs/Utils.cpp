#include "Utils.hpp"

std::string	Utils::cut_message(std::string &s, bool remove_endline, int end) {
    std::string ret;

    ret = s.substr(0, s.find('\n') + end);
    if (remove_endline == true)
        s.erase(0, s.find('\n') + 1);
    return (ret);
}

std::string Utils::itoa(int a) {
    std::string res;
    while(a)
    {
        int x = a % 10;
        a /= 10;
        char i = '0';
        i = i + x;
        res = i + res;
    }
    return res;
}

void	Utils::remove_whitespace(std::string &s, int i) {
	while (std::iswspace(s[i]))
		s.erase(s.begin() + i);
}