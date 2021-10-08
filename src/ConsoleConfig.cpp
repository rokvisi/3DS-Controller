#include "ConsoleConfig.h"
#include <3ds.h>
#include <iostream>
#include "internet.h"

ConsoleConfig::~ConsoleConfig() {
    if (is_GFX()) {
        gfxExit();
    }
    if (is_PTMU()) {
        ptmuExit();
    }
    if (is_ROMFS()) {
        romfsExit();
    }
    if (is_SOCKET()) {
        socExit();
    }

    
}

void ConsoleConfig::initialize() {
    if (is_GFX())        { gfxInitDefault();                 } //? Graphics
    if (is_PTMU())       { ptmuInit();                       } //? PTMU (battery level, state...)
    if (is_ROMFS())      { romfsInit();                      } //? Filesystem
    if (is_SOCKET())     { socket_init();                    } //? Internet
    if (is_SCR_TOP())    { consoleInit(GFX_TOP, nullptr);    } //? Top screen
    if (is_SCR_BOTTOM()) { consoleInit(GFX_BOTTOM, nullptr); } //? Bottom screen
}

bool ConsoleConfig::is_GFX()        { return state[0]; }
bool ConsoleConfig::is_PTMU()       { return state[1]; }
bool ConsoleConfig::is_ROMFS()      { return state[2]; }
bool ConsoleConfig::is_SOCKET()     { return state[3]; }
bool ConsoleConfig::is_SCR_TOP()    { return state[4]; }
bool ConsoleConfig::is_SCR_BOTTOM() { return state[5]; }

ConsoleConfig& ConsoleConfig::set_GFX()        { state[0] = true; return *this; }
ConsoleConfig& ConsoleConfig::set_PTMU()       { state[1] = true; return *this; }
ConsoleConfig& ConsoleConfig::set_ROMFS()      { state[2] = true; return *this; }
ConsoleConfig& ConsoleConfig::set_SOCKET()     { state[3] = true; return *this; }
ConsoleConfig& ConsoleConfig::set_SCR_TOP()    { state[4] = true; return *this; }
ConsoleConfig& ConsoleConfig::set_SCR_BOTTOM() { state[5] = true; return *this; }