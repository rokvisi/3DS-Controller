#include "io.h"

#include <iostream>
#include <fstream>
#include "utils.h"

// TODO: Add handling for ill-formed files
//? The last string is a potential error messages
std::pair<settings_t, std::string> read_settings(std::string_view path) {
    std::ifstream input_file(path.data());
	if(!input_file.is_open()) {
		return std::make_pair(settings_t{}, std::string{"Could not open:\n\t\""} + path.data() + "\"\n\nDefaulting to:\n\t");
	}

    std::string ip, port;
	input_file >> ip >> port;

    return std::make_pair(std::make_pair(ip, port), "");
}