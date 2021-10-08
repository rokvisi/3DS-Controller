#include "utils.h"

#include <unistd.h>

bool is_null_terminated(char* str, const unsigned int max_size) {
    for (unsigned int i = 0; i < max_size; ++i) {
        if (str[i] == '\0') {
            return true;
        }
    }

    return false;
}

std::pair<std::string, std::string> get_pwd() {
    static const unsigned int MAX_PATH_SIZE = 512;

    char buffer[MAX_PATH_SIZE] = {0};
    char* const result = getcwd(buffer, MAX_PATH_SIZE);
    //TODO: Check for 'getcwd() shall return a null pointer and set errno to indicate the error.'

    if (result == nullptr) {
        return std::make_pair("", "Unable to get present working directory. No filesystem access?");
    }
    if (!is_null_terminated(result, MAX_PATH_SIZE)) {
        return std::make_pair("", "Present working directory exceeds 512 bytes. How?");
    }

    return std::make_pair(result, "");
}

