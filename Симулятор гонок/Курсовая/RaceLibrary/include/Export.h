#pragma once

#ifdef _WIN32
    #ifdef RACELIBRARY_EXPORTS
        #define RACE_API __declspec(dllexport)
    #else
        #define RACE_API __declspec(dllimport)
    #endif
#else
    #define RACE_API
#endif