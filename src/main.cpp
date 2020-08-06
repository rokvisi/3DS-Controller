#include "../include/internet.h"

//* Standard c++ headers
#include <iostream>
#include <algorithm>
#include <memory>
#include <fstream>
#include <thread>

//* Linux headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

//* 3DS headers
#include <3ds.h>

const std::string DEFAULT_IP("192.168.8.103"), DEFAULT_PORT("8889");

// TODO: Add handling for ill-formed files
/*
 * @brief Reads the settings.ini file located on the SD card and parses the ip and port numbers.	
 * 
 * @param ip The string that will hold the ip address
 * @param port The string that will hold the port number
*/
void read_settings(std::string& ip, std::string& port) {
	std::ifstream input_file("controller.ini");
	if(!input_file.is_open()) {
		std::cout << "Could not open the settings file!\n";
		return;
	}

	input_file >> ip >> port;
}

/*
 * @brief Refreshes the key states and returns them in an array.
 * 
 * @return Up to date key states
*/
std::array<uint32_t, 3> get_key_states() {
	hidScanInput();
	return {hidKeysDown(), hidKeysHeld(), hidKeysUp()};
}

/*
 * @brief Gets the battery level
 * 
 * @return Battery level indication in the range of [0; 5]
*/
uint8_t get_battery_level() {
	uint8_t result;
	PTMU_GetBatteryLevel(&result);

	return result;
}

/*
 * @brief Gets the current charging state
 * 
 * @return 1 - Charging, else - not charging
*/
uint8_t get_charging_state() {
	uint8_t result;
	PTMU_GetBatteryChargeState(&result);

	return result;
}

/*
 * @brief Flushes and swaps the frame buffers. Waits for VBlank.
*/
void update_screen() {
	//* Flush and swap framebuffers
	gfxFlushBuffers();
	gfxSwapBuffers();
		
	//* Wait for VBlank
	gspWaitForVBlank();
}

/*
 * @brief The program's entry point
 * 
 * @param argc Argument count
 * @param argv Argument string array
 * @return Error code
 */
int main(int argc, char* argv[]) {
	//* Initialize necessary sub-systems
	gfxInitDefault(); 				  //? Graphics
	ptmuInit();      				  //? PTMU (battery level, state...)
	romfsInit(); 				      //? Filesystem
	socket_init(); 					  //? Internet
	consoleInit(GFX_BOTTOM, nullptr); //? Bottom screen

	//* Read the desired ip and port from the settings file
	std::string ip(DEFAULT_IP), port(DEFAULT_PORT);
	read_settings(ip, port);

	//* Connect to the server
	UDP_Client connection(ip, port);
	std::cout << "\x1b[2;1H" << ip << " on " << port;

	//* Declare variables
	std::array<uint32_t, 3> input_states;
	uint8_t battery_level(0), charging_state(0);

	//* Main loop
	while(aptMainLoop()) {
		//* Update the states and send them over if needed
		if(auto new_states = get_key_states(); input_states != new_states) {
			input_states = new_states;

			connection.send(reinterpret_cast<char*>(input_states.data()), sizeof(input_states));
		}

		//* Update battery text
		if(auto new_level = get_battery_level(), new_state = get_charging_state(); new_level != battery_level || new_state != charging_state) {
			battery_level = new_level;
			charging_state = new_state;

			const char charging_symbol = (charging_state == 1 ? static_cast<char>(254) : ' ');			
			std::cout << "\x1b[4;1H" << "Battery: " << static_cast<size_t>(battery_level) << "/5" << charging_symbol;

			update_screen();
		}
	}
}
