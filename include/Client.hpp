#ifndef CLIENT_HPP
# define CLIENT_HPP

class Client;
typedef std::map<std::string, Client>   client_map;

class   Client
{
    public:

        Client(void);
        ~Client(void);

        void    setFd(const int & fd);

    private:

        int _clientFd;

};

#endif