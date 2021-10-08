#pragma once

#include <string>
#include <tuple>
#include "3ds_utils.h"

//? Functions declared in this file are meant to be generic c++ functions.

bool is_null_terminated(char* str, const unsigned int max_size);

std::pair<std::string, std::string> get_pwd();

//* Unwraps a std::pair<T, std::string (error)> and returns the T if all is well.
template <typename T>
T unwrap(const std::pair<T, std::string>& result) {
    auto [val, error] = result;
    handle_error(error);

    return val;
}
