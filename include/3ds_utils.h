#pragma once

#include <array>
#include <tuple>
#include <string_view>
#include <cstdint>

//? Functions declared in this file are meant to be generic 3ds functions.

/*
 * @return Retruns true if the application was loaded from local storage. False if something like 3dslink was used.
*/
std::pair<bool, std::string> is_local_loaded();

std::pair<std::string_view, std::string> get_app_specific_homebrew_data_dir();

/*
 * @brief Refreshes the key states and returns them in an array.
 *
 * @return Up to date key states.
*/
std::array<uint32_t, 3> get_key_states();

/*
 * @brief Gets the battery level
 *
 * @return Battery level indication in the range of [0; 5]
*/
uint8_t get_battery_level();

/*
 * @brief Gets the current charging state
 *
 * @return 1 - Charging, else - not charging
*/
uint8_t get_charging_state();

/*
 * @brief Flushes and swaps the frame buffers. Waits for VBlank.
*/
void update_screen();

void handle_error(const std::string& error);
