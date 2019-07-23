#include "ConfigManager.hpp"

// we instanciating this, we want to see if the file is present
ConfigData::ConfigData()
{
    filesystem::path path = "/input-recorder/config.ini";
    if (!filesystem::exists(path)) {
        this->mMode = std::make_tuple<RecordMode, int>(IMMEDIATE, 0);

        this->mControllersToRecord = {CONTROLLER_P1_AUTO,  CONTROLLER_PLAYER_2,
                                      CONTROLLER_PLAYER_3, CONTROLLER_PLAYER_4,
                                      CONTROLLER_PLAYER_5, CONTROLLER_PLAYER_6,
                                      CONTROLLER_PLAYER_7, CONTROLLER_PLAYER_8};
        this->WriteConfig();
    }
}

void ConfigData::WriteConfig() const
{
    using namespace simpleIniParser;

    Ini *ini = new Ini();

    IniSection *controllerSection = new IniSection(SECTION, "controllers");
    for (auto &i : this->mControllersToRecord) {
        controllerSection->options.push_back(
            new IniOption(controllerToString.at(i), "1"));
    }

    delete ini;
}

ConfigData::~ConfigData() {}