#pragma once

#include <string>
#include <tuple>

//? Functions declared in this file are meant to be program specific IO functions.

/*
 * @brief Reads the settings.ini file located on the SD card and parses the ip and port numbers.
 *
 * @param path The path to the settings file
*/
//? The last string is a potential error message
using settings_t = std::pair<std::string, std::string>;

std::pair<settings_t, std::string> read_settings(std::string_view path);