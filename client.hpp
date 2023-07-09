#ifndef CLIENT_HPP
#define CLIENT_HPP

class Client {
	private:
		int 		fd;
		std::string nick;
		std::string real_name;
		std::string user_name;
		std::string server_name;
		std::vector<char> buff(5000);
	public:
		Client();
		Client( int );
		~Client();
};

#endif