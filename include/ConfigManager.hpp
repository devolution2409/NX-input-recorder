#pragma once
#include "helpers/Logger.hpp"
#include "helpers/SimpleIniParser/SimpleIniParser.hpp"
#include "helpers/Singleton.hpp"

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <switch.h>
#include <tuple>
#include <vector>

#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#    if __has_include(<filesystem>)
#        include <filesystem>
namespace filesystem = std::filesystem;
#    elif __has_include(<experimental/filesystem>)
#        include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#    elif __has_include(<boost/filesystem.hpp>)
#        include <boost/filesystem.hpp>
namespace filesystem = boost::filesystem;
#    endif
#endif

enum RecordMode {
    IMMEDIATE,
    INPUT_BASED,
    TIME_BASED,
    FRAME_BASED =
        TIME_BASED // just an alias. It's still implied that the number saved in
                   // the config file is in seconds. So 60 frames.
};

// class holding the actual data, that we want to be a singleton
class ConfigData
{
  public:
    ConfigData();
    ~ConfigData();

    inline std::vector<HidControllerID> GetController() const
    {
        return this->mControllersToRecord;
    };

    inline RecordMode GetRecordMode() const
    {
        return std::get<0>(this->mMode);
    };
    inline int GetRecordOption() const { return std::get<1>(this->mMode); };

    inline void SetRecordMode(RecordMode mode, int options)
    {
        // std::move to fix cannot bind rvalue int&& to lvalue int
        // FeelsDankMan i have no idea what this means
        this->mMode = std::make_tuple<RecordMode, int>(std::move(mode),
                                                       std::move(options));
    };
    inline void SetRecordMode(std::tuple<RecordMode, int> other)
    {
        this->mMode = other;
    };

    void ReadConfig();

  private:
    void WriteConfig();
    void WriteInitialConfig();
    std::vector<HidControllerID> mControllersToRecord;
    // basically int will the be option. if record mode == immediate we dont
    // care. if it is inputBased it's the number of input if it's time it will
    // be the number of seconds etc
    std::tuple<RecordMode, int> mMode;

    // todo: better way to do this? it feels lidl
    const std::map<HidControllerID, std::string> controllerToString = {
        {CONTROLLER_P1_AUTO, "CONTROLLER_P1_AUTO"},
        {CONTROLLER_PLAYER_2, "CONTROLLER_PLAYER_2"},
        {CONTROLLER_PLAYER_3, "CONTROLLER_PLAYER_3"},
        {CONTROLLER_PLAYER_4, "CONTROLLER_PLAYER_4"},
        {CONTROLLER_PLAYER_5, "CONTROLLER_PLAYER_5"},
        {CONTROLLER_PLAYER_6, "CONTROLLER_PLAYER_6"},
        {CONTROLLER_PLAYER_7, "CONTROLLER_PLAYER_7"},
        {CONTROLLER_PLAYER_8, "CONTROLLER_PLAYER_8"},
    };

    const std::map<std::string, HidControllerID> stringToController = {
        {"CONTROLLER_P1_AUTO", CONTROLLER_P1_AUTO},
        {"CONTROLLER_PLAYER_2", CONTROLLER_PLAYER_2},
        {"CONTROLLER_PLAYER_3", CONTROLLER_PLAYER_3},
        {"CONTROLLER_PLAYER_4", CONTROLLER_PLAYER_4},
        {"CONTROLLER_PLAYER_5", CONTROLLER_PLAYER_5},
        {"CONTROLLER_PLAYER_6", CONTROLLER_PLAYER_6},
        {"CONTROLLER_PLAYER_7", CONTROLLER_PLAYER_7},
        {"CONTROLLER_PLAYER_8", CONTROLLER_PLAYER_8},
    };

    const std::string filename = "sdmc:/input-recorder/config.ini";
};

struct ConfigManager : public Singleton<ConfigData, ConfigManager> {

    // Call the Singleton constructor with an int for it to invoke your static
    // init() method.

    ConfigManager() : Singleton() {}

    // static AsyncLogger *init()
    // {
    //     // actually instanciate because the instance of the Singleton
    //     template
    //     // will be equal to what we return here
    //     FILE *p = fopen("/input-recorder/logs.txt", "w");
    //     return new AsyncLogger(p);
    // };
};
