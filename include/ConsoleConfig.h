#pragma once
#include <bitset>

class ConsoleConfig {
    public:
        ~ConsoleConfig();

        void initialize();

        // Getters
        bool is_GFX();
        bool is_PTMU();
        bool is_ROMFS();
        bool is_SOCKET();
        bool is_SCR_TOP();
        bool is_SCR_BOTTOM();

        // Setters
        ConsoleConfig& set_GFX();
        ConsoleConfig& set_PTMU();
        ConsoleConfig& set_ROMFS();
        ConsoleConfig& set_SOCKET();
        ConsoleConfig& set_SCR_TOP();
        ConsoleConfig& set_SCR_BOTTOM();

    private:
        std::bitset<6> state;
};