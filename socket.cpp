#include <string>
#include <iostream>
#include <vector>
#include <functional>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define _DEBUG

namespace Log {
    template<class ...Args>
        auto Line(Args const& ... args) {
        #ifdef _DEBUG
            ((std::cout << args), ...);
            std::cout << '\n';
        #endif
        }

    template<class ...Args>
        auto Error(Args const& ... args) {
            Line("ERROR   :: ", args...);
        }

    auto ErrorEx(const char* msg, const char* file, const char* line) {
            Line("ERROR   :: ", msg, "; File: ", file, "; Line: ", line, ";");
        }

    template<class ...Args>
        auto Warning(Args const& ... args) {
            Line("WARNING :: ", args...);
        }
}

namespace WW {
/* 
    sd - socket descriptor
*/

    using sock_t = int;

    auto CreateSocket(std::string address, std::string port) {
        addrinfo* addr    = nullptr;
        sock_t sock_sd    = 0;
        addrinfo tmp;
        tmp.ai_family     = PF_INET;
        tmp.ai_flags      = AI_CANONNAME;
        tmp.ai_protocol   = IPPROTO_IP;
        tmp.ai_socktype   = SOCK_STREAM;

        if ( !getaddrinfo(address.data(), port.data(), &tmp, &addr) ) {
            sock_sd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        } else Log::ErrorEx("getaddrinfo()", __FILE__, __LINE__);

        return sock_sd;
    }

    auto CloseSocket(sock_t sd) {
        close(sd);
    }

    auto SendData(sock_t sd, const void* data, size_t lenght, uint32_t flags = 0) {
        int total = 0
          , n     = 0;

        while( total < len && n != -1 ) {
            n = send(sd, data, len - total, flags);
            total += n;
        }

        return (n == -1) ? n : total;
    }

    auto RecieveData(sock_t sd, void *buffer, int len, int flags) {
        if ( !buffer ) {  
            Log::ErrorEx("buffer is nullptr", __FILE__, __LINE__);
        }

        int total = 0
          , n     = 0;

        while(total < len && n != -1) {
            n = recv(sd, buffer, len - total, flags);
            total += n;
        }

        return (n == -1) ? n : total;
    }

    auto Listen(sock_t sd, int queue) {
        while ( !listen(sd, queue) ) {

        }
    }

    struct SockClient {
        using value_type   = sock_t;
        using buffer_type  = std::vector<char>;
        using callback_fun = std::function<void(SockClient&)>;

        int sequence_data { 1024 };

        buffer_type buffer;
        value_type  sd { -1 }; // socket descriptor

        callback_fun data_update;
    };

    auto Bind(sock_t sd, int port, int addr = hton(INADDR_ANY) ) {
        sockaddr_in tmp;
        tmp.sin_family = PF_INET;
        tmp.sin_port   = htons(port);
        
    }
}

int main(int argc, char *argv[]) {
    std::vector<WW::SockClient> Clients; // набор клиентов
    
    auto SOCKET = WW::CreateSocket("localhost", "9001");

    Log::Line("SOCKET create; sd: ", SOCKET);

    

    getchar();

    WW::CloseSocket(SOCKET);
    return 0;
}