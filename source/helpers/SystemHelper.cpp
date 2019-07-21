#include "SystemHelper.hpp"

namespace Helper {

namespace System {

u64 GetActiveApplicationProcessID()
{

    u64 pid;
    try {
        if (R_FAILED(pmdmntGetApplicationPid(&pid))) {
            throw(std::runtime_error("Couldn't get pid"));
        }
    }
    catch (...) {
        // rethrows to control flow
        throw;
    }
    return pid;
}

u64 GetActiveTitleID()
{
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
        // rethrows to control flow
        throw;
    }
    return title;
}

} // namespace System
} // namespace Helper