#include <citro2d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


const size_t SCREEN_WIDTH = 400;
const size_t SCREEN_HEIGHT = 240;

const int INVALID_SOCKET = ~0;
const int SOCKET_ERROR = -1;

struct Pos {
	Pos() : x(0), y(0) {}
	Pos(const size_t x, const size_t y) : x(x), y(y) {}

	size_t x;
	size_t y;
};

void print(const Pos pos, const std::string& text) {
	// Check for invalid sizes
	if(pos.x < 0 || pos.y <= 0) {
		return;
	}

	const std::string prefix = "\x1b[" + std::to_string(pos.y) + ';' + std::to_string(pos.x) + 'H';
	std::cout << prefix + text;
}
/*
int main(int argc, char* argv[]) {
	// Init libs
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);

	// Main loop
	while (aptMainLoop()) {
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break;
		}

		if (kDown & KEY_SELECT) {
			print({13, 13}, "SELECT IS PRESSED!");
		}

		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		std::cout << "\x1b[5;240HTest!\x1b[K";
		print({0, 6}, "Hello!");
	}

	// Deinit libs
	gfxExit();
	return 0;
}
*/
/*
int main(int argc, char* argv[]) {
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);

    // Create, clear then set the 'hints' struct member
    addrinfo hints, *result(0);
	memset((void*)&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;

	//const std::string IP("192.168.8.102"), PORT("27215");
	const char* i = "192.168.8.102";
	const char* p = "8889";
    int error = getaddrinfo(i, p, &hints, &result);
    if(error != 0) {
		std::cout << "GetAddrInfo failed: ";

		const char* errstr = gai_strerror(error);
		if(errstr == nullptr) {
			std::cout << "What the fuck.";
		}
		else {
			std::cout << gai_strerror(error);
		}
	
    	return -1;
	}

	print({1, 6}, "");

    // Create a socket for making a connection and check for errors
    int ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(ConnectSocket == INVALID_SOCKET) { std::cout << "Error at socket(): "; freeaddrinfo(result); return 0; }

    // Try to connect to the server and check for errors
    error = connect(ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
    if(error == SOCKET_ERROR) { closesocket(ConnectSocket); std::cout << "Unable to connect to server!"; return 0; }
    freeaddrinfo(result);
/*
    char recvbuf[DEFAULT_BUFLEN];  const char *sendbuf = "Fuck you bruv";

    // Attempt to send a message and check for errors
    std::cout << "Sending: \"" << sendbuf << "\"";
    error = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if(error == SOCKET_ERROR) { cout << "Send failed: "; closesocket(ConnectSocket); return 0; }

    std::cout << "Bytes sent: " << error << '\n';

    error = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);

    std::string message;
    while(message != "n") {
        std::cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> message;

        int sent_bytes = send(ConnectSocket, message.data(), message.size(), 0);
        if(sent_bytes == SOCKET_ERROR) {
            cout << "Send failed: ";
            WSAGetLastError();
            closesocket(ConnectSocket);
            WSACleanup();
            getch();
            return 0;
        }
    }
----------------
	const std::string message("Sent from the 3DS!");
	const int sent_bytes = send(ConnectSocket, message.data(), message.size(), 0);
	if(sent_bytes == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		return 0;
	}

	while (aptMainLoop()) {
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break;
		}

		if (kDown & KEY_SELECT) {
			print({13, 13}, "SELECT IS PRESSED!");
		}

		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		std::cout << "\x1b[5;240HTest!\x1b[K";
		print({0, 6}, "Hello!");
	}

	gfxExit();
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void check_host_name(int hostname) { //This function returns host name for local computer
   if (hostname == -1) {
      perror("gethostname");
      exit(1);
   }
}
void check_host_entry(struct hostent * hostentry) { //find host info from host name
   if (hostentry == NULL){
      perror("gethostbyname");
      exit(1);
   }
}
void IP_formatter(char *IPbuffer) { //convert IP string to dotted decimal format
   if (NULL == IPbuffer) {
      perror("inet_ntoa");
      exit(1);
   }
}
int main(int argc, char* argv[]) {
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);
	
   char host[256];
   struct hostent *host_entry;

   int hostname = gethostname(host, sizeof(host)); //find the host name
   check_host_name(hostname);

	std::cout << host;

   host_entry = gethostbyname(host); //find host information
   check_host_entry(host_entry);
   /*
   char* IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
   printf("Current Host Name: %s\n", host);
   printf("Host IP: %s\n", IP);*/
}