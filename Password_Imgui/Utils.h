#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

std::string GetExecutablePath() {
    char buffer[MAX_PATH] ;
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

#endif
