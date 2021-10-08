#include <memory>
#include <string>

#ifdef _3DS
    #include <citro2d.h>
#endif

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>

    typedef std::unique_ptr<addrinfo, void(__stdcall*)(addrinfo*)> addrinfo_unique_ptr;
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>

    const int INVALID_SOCKET = ~0;
    const int SOCKET_ERROR = -1;

    typedef std::unique_ptr<addrinfo, void(*)(addrinfo*)> addrinfo_unique_ptr;
#endif

#ifndef _WIN32
    std::string errno_s();
#endif

/*
 * @brief Initializes the use of sockets. Winsock for windows, allocation for the 3ds and nothing for basic linux.
*/
void socket_init();

addrinfo_unique_ptr get_tcp_addrinfo(const std::string& ip, const std::string& port);
addrinfo_unique_ptr get_udp_addrinfo(const std::string& ip, const std::string& port);

class UDP_Server {
    public:
        UDP_Server(const std::string& port);

        int recv(char* const data, const size_t size);
        int recv(std::string& message);

    private:
        int connection;
        addrinfo_unique_ptr addr_info;
};

class UDP_Client {
	public:
		UDP_Client(const std::string& ip, const std::string& port);

		int send(const char* const data, const size_t size);
		int send(const std::string& message);

	private:
		int connection;
		addrinfo_unique_ptr addr_info;
};