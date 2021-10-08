#include "3ds_utils.h"

#include <iostream>
#include <3ds.h>
#include "utils.h"

std::pair<bool, std::string> is_local_loaded() {
    auto [pwd, error] = get_pwd();
    if (!error.empty()) {
        return std::make_pair(bool{}, error);
    }

    return std::make_pair(pwd.starts_with("sdmc:"), "");
}

std::pair<std::string_view, std::string> get_app_specific_homebrew_data_dir() {
    auto [local_loaded, error] = is_local_loaded();
    if (!error.empty()) {
        return std::make_pair("", error);
    }

    std::cout << "origin: " << (local_loaded ? "local storage" : "3DSLink") << '\n';
    return std::make_pair(local_loaded ? "data/HandController/" : "3DS/data/HandController/", "");
}

std::array<uint32_t, 3> get_key_states() {
	hidScanInput();
	return { hidKeysDown(), hidKeysHeld(), hidKeysUp() };
}

uint8_t get_battery_level() {
	uint8_t result;
	PTMU_GetBatteryLevel(&result);

	return result;
}

uint8_t get_charging_state() {
	uint8_t result;
	PTMU_GetBatteryChargeState(&result);

	return result;
}

void update_screen() {
	//* Flush and swap framebuffers
	gfxFlushBuffers();
	gfxSwapBuffers();

	//* Wait for VBlank
	gspWaitForVBlank();
}

void handle_error(const std::string& error) {
    //? If no error occured.
    if (error.empty()) return;

    //* Display the error and quit.
    std::cout << "Error: " << error << "\n\nPress START to exit.\n";
    update_screen();

    //* Waits for start to be pressed, then quits.
    while(aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_START) {
            exit(-1);
        }
    }
}