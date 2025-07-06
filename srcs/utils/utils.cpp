#include "Utils.hpp"

void    addClient(client_map & clients, Client const & client, int const & clientSocket)
{
    // for(int i = 0; i < std::numeric_limits<int>::max(); i++)
    // {
    //     std::stringstream ss;
    //     std::string key;
    //     ss << i;
    //     ss >> key;
    //     client_map::const_iterator it = clients.find(key);
    //     if (it != clients.end())
    //         continue;
        clients.insert(std::pair<int, Client>(clientSocket, client));
    //     clients[clientSocket].setUid(key);
    //     break ;
    // }
}

bool    isServerSocket(int const & serverSocket, serv_vector const & servers)
{
    for (size_t i = 0; i < servers.size(); i++)
    {
        if (serverSocket == servers[i].getSocket())
            return true;
    }
    return false;
}