#pragma once
#include "helpers/AsyncLogger.hpp"
#include "helpers/Singleton.hpp"

struct Logger : public Singleton<AsyncLogger, Logger> {

    // Call the Singleton constructor with an int for it to invoke your static
    // init() method.

    Logger() : Singleton(1) {}

    static AsyncLogger *init()
    {
        // actually instanciate because the instance of the Singleton template
        // will be equal to what we return here
        FILE *p = fopen("/input-recorder/logs.txt", "w");
        return new AsyncLogger(p);
    };
};
