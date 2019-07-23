#include "SystemHelper.hpp"

namespace Helper {

namespace System {

u64 GetActiveApplicationProcessID()
{
    Logger logger;
    logger->trace("Getting active Application pID\n");
    u64 pid;
    try {
        if (R_FAILED(pmdmntGetApplicationPid(&pid))) {
            throw(std::runtime_error("Couldn't get pid"));
        }
    }
    catch (...) {
        logger->error("Failed to get pID\n");
        // rethrows to control flow
        throw;
    }
    return pid;
}

u64 GetActiveTitleID()
{
    Logger logger;
    logger->trace("Attempting to get Title ID\n");
    u64 title;
    u64 pid;
    try {
        pid = Helper::System::GetActiveApplicationProcessID();
    }
    catch (std::exception &e) {
        // https://stackoverflow.com/questions/6755991/catching-stdexception-by-reference
        // rethrow in case we fail to get the pid, so that we know we need to
        // work in unknown folder
        throw(e);
    }

    try {
        if (R_FAILED(pminfoGetTitleId(&title, pid))) {
            throw(std::runtime_error("Couldn't get titleID"));
        }
    }
    catch (...) {
        logger->error("Couldn't get title ID");
        // rethrows to control flow
        throw;
    }
    return title;
}

} // namespace System
} // namespace Helper