#include "internet.h"

#include <iostream>
#include <string.h>

#ifndef _WIN32
	/*
	 * @brief Gets the latest system error string representation.
	 *
	 * @return The string representation of **errno** provided by strerror_r()
	*/
	std::string errno_s() {
	    char buff[BUFSIZ];
	    strerror_r(errno, buff, BUFSIZ);

	    return buff;
	}
#endif

/*
 * @brief Returns a filled, smart addrinfo struct pointer that was created with these hints:
 * ai_family = AF_UNSPEC,
 * ai_socktype = SOCK_STREAM,
 * ai_protocol = IPPROTO_TCP
 *
 * @param ip The desired ip
 * @param port The desired port
 * @return A std::unique_ptr to the filled addrinfo struct
*/
addrinfo_unique_ptr get_tcp_addrinfo(const std::string& ip, const std::string& port) {
	addrinfo hints = {0}, *result;
	hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

	int error = getaddrinfo(!ip.empty() ? ip.data() : nullptr, !port.empty() ? port.data() : nullptr, &hints, &result);
	if(error) {
		#ifdef _WIN32
			std::cout << "GetAddrInfo failed: " << WSAGetLastError();
		#else
			std::cout << "GetAddrInfo failed: " << gai_strerror(errno);
		#endif
		exit(-1);
	}

	return addrinfo_unique_ptr(result, &::freeaddrinfo);
}

/*
 * @brief Returns a filled, smart addrinfo struct pointer that was created with these hints:
 * ai_family = AF_INET,
 * ai_socktype = SOCK_DGRAM,
 * ai_protocol = IPPROTO_UDP,
 * ai_flags = AI_PASSIVE
 *
 * @param ip The desired ip
 * @param port The desired port
 * @return A std::unique_ptr to the filled addrinfo struct
*/
addrinfo_unique_ptr get_udp_addrinfo(const std::string& ip, const std::string& port) {
	addrinfo hints = {0}, *result;
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    hints.ai_flags = AI_PASSIVE;

	int error = getaddrinfo(!ip.empty() ? ip.data() : nullptr, !port.empty() ? port.data() : nullptr, &hints, &result);
	if(error) {
		#ifdef _WIN32
			std::cout << "GetAddrInfo failed: " << WSAGetLastError();
		#else
			std::cout << "GetAddrInfo failed: " << gai_strerror(errno);
		#endif
		exit(-1);
	}

	return addrinfo_unique_ptr(result, &::freeaddrinfo);
}

#ifdef _WIN32
    void socket_init() {
        WSADATA wsa_data;
        WORD requested_version = MAKEWORD(2, 2);

        if(int error = WSAStartup(requested_version, &wsa_data); error) {
            std::cout << "WSAStartup failed! Error " << error;
            exit(-1);
        }
    }
#elif _3DS
    void socket_init() {
        const size_t SOC_ALIGN      = 0x1000;
	    const size_t SOC_BUFFERSIZE = 0x100000;

	    uint32_t* socket_buffer = (uint32_t*)aligned_alloc(SOC_ALIGN, SOC_BUFFERSIZE);
	    if(socket_buffer == nullptr) {
	    	std::cout << "memalign: Failed to allocate the socket buffer!";
        	exit(-1);
        }

	    if(auto error = socInit(socket_buffer, SOC_BUFFERSIZE); error) {
	        std::cout << "socInit failed!";
	        exit(-1);
	    }
    }
#endif

UDP_Server::UDP_Server(const std::string& port) : connection(0), addr_info(nullptr, &::freeaddrinfo) {
    addr_info = get_udp_addrinfo("", port);

    connection = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
    if(connection == INVALID_SOCKET) {
        std::cout << "socket() failed! -> ";

		#ifdef _WIN32
			std::cout << WSAGetLastError();
		#else
			std::cout << errno_s();
		#endif
    }

    int error = bind(connection, addr_info->ai_addr, addr_info->ai_addrlen);
    if(error == SOCKET_ERROR) {
        std::cout << "bind() failed! ->";

		#ifdef _WIN32
			std::cout << WSAGetLastError();
		#else
			std::cout << errno_s();
		#endif
    }
}
int UDP_Server::recv(char* const data, const size_t size) {
    #ifdef _WIN32
        return recvfrom(connection, data, size, 0, addr_info->ai_addr, reinterpret_cast<int*>(&addr_info->ai_addrlen));
    #else
        return recvfrom(connection, data, size, 0, addr_info->ai_addr, &addr_info->ai_addrlen);
    #endif
}
int UDP_Server::recv(std::string& message) {
    #ifdef _WIN32
        return recvfrom(connection, &message[0], message.size(), 0, addr_info->ai_addr, reinterpret_cast<int*>(&addr_info->ai_addrlen));
    #else
        return recvfrom(connection, &message[0], message.size(), 0, addr_info->ai_addr, &addr_info->ai_addrlen);
    #endif
}

UDP_Client::UDP_Client(const std::string& ip, const std::string& port) : connection(0), addr_info(nullptr, freeaddrinfo) {
	addr_info = get_udp_addrinfo(ip, port);

	connection = socket(addr_info->ai_family, addr_info->ai_socktype, addr_info->ai_protocol);
	if(connection == INVALID_SOCKET) {
		std::cout << "socket() failed -> ";

		#ifdef _WIN32
			std::cout << WSAGetLastError();
		#else
			std::cout << errno_s();
		#endif
	}
}
int UDP_Client::send(const std::string& message) {
	return ::sendto(connection, message.data(), message.size(), 0, addr_info->ai_addr, addr_info->ai_addrlen);
}
int UDP_Client::send(const char* const data, const size_t size) {
	return ::sendto(connection, data, size, 0, addr_info->ai_addr, addr_info->ai_addrlen);
}