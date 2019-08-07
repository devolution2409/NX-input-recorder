#include "SystemHelper.hpp"

namespace Helper
{

namespace System
{

u64 GetActiveApplicationProcessID()
{
    Logger logger;
    logger->trace("Getting active Application pID\r\n");
    u64 pid;
    try {
        if (R_FAILED(pmdmntGetApplicationPid(&pid))) {
            throw(std::runtime_error("Couldn't get pid"));
        }
    } catch (...) {
        logger->error("Failed to get pID\r\n");
        // rethrows to control flow
        throw;
    }
    return pid;
}

u64 GetActiveTitleID()
{
    Logger logger;
    logger->trace("Attempting to get Title ID\r\n");
    u64 title;
    u64 pid;
    try {
        pid = Helper::System::GetActiveApplicationProcessID();
    } catch (std::exception &e) {
        // https://stackoverflow.com/questions/6755991/catching-stdexception-by-reference
        // rethrow in case we fail to get the pid, so that we know we need to
        // work in unknown folder
        throw(e);
    }

    try {
        if (R_FAILED(pminfoGetTitleId(&title, pid))) {
            throw(std::runtime_error("Couldn't get titleID"));
        }
    } catch (...) {
        logger->error("Couldn't get title ID\r\n");
        // rethrows to control flow
        throw;
    }
    return title;
}

std::vector<HidControllerID> GetConnectedControllers()
{
    std::vector<HidControllerID> result;
    std::vector<HidControllerID> controllerList = {
        CONTROLLER_P1_AUTO,  CONTROLLER_PLAYER_2, CONTROLLER_PLAYER_3,
        CONTROLLER_PLAYER_4, CONTROLLER_PLAYER_5, CONTROLLER_PLAYER_6,
        CONTROLLER_PLAYER_7, CONTROLLER_PLAYER_8,
    };

    for (auto &i : controllerList)
        if (hidIsControllerConnected(i))
            result.push_back(i);

    return result;
    // hidIsControllerConnected (HidControllerID id)
} // namespace System

} // namespace System
} // namespace Helper