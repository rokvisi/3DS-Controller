#include "internet.h"
#include "ConsoleConfig.h"
#include "utils.h"
#include "3ds_utils.h"
#include "io.h"

//* Standard c++ headers
#include <iostream>

//* 3DS headers
#include <3ds.h>

//TODO: Add a constants file to have a app_name then change io::read_settings() to use it.

int main(int argc, char* argv[]) { //* Test comment from remote commit.
    //* Config the things.
    ConsoleConfig config;
    config.set_GFX().set_PTMU().set_ROMFS().set_SOCKET().set_SCR_TOP().initialize();

    //* Determine how the app was launched to get the correct path when reading the settings file.
    auto data_dir = unwrap(get_app_specific_homebrew_data_dir());

    //* Read the desired ip and port from the settings file. Connect to the server.
    auto [ip, port] = unwrap(read_settings(std::string{data_dir} + "controller.ini"));
    UDP_Client connection(ip, port);

    std::cout << ip << " on " << port << '\n';

	//* Declare variables.
	std::array<uint32_t, 3> input_states;
	uint8_t battery_level(0), charging_state(0);

	//* Main loop.
	while(aptMainLoop()) {
		//* Update the states and send them over if needed
		if(auto new_states = get_key_states(); input_states != new_states) {
            //* Quit the app on KEY_START press.
            if (new_states[0] & KEY_START) {
                break;
            }

			input_states = new_states;
			connection.send(reinterpret_cast<char*>(input_states.data()), sizeof(input_states));
		}

		//* Update battery text
		if(auto new_level = get_battery_level(), new_state = get_charging_state(); new_level != battery_level || new_state != charging_state) {
			battery_level = new_level;
			charging_state = new_state;

			const char charging_symbol = (charging_state == 1 ? static_cast<char>(254) : ' ');
			std::cout << "\x1b[30;1H" << "Battery: " << static_cast<size_t>(battery_level) << "/5" << charging_symbol;

			update_screen();
		}
	}
}
