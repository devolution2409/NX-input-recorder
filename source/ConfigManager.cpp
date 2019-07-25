#include "ConfigManager.hpp"

// we instanciating this, we want to see if the file is present
ConfigData::ConfigData()
{
    Logger logger;

    filesystem::path path = "/input-recorder/config.ini";
    if (!filesystem::exists(path)) {

        this->WriteInitialConfig();
    } else {
        this->ReadConfig();
    }
}

void ConfigData::WriteInitialConfig()
{
    Logger logger;
    logger->trace("Writing default config file.\r\n");

    this->SetRecordMode(std::make_tuple(RecordMode::IMMEDIATE, 0));

    this->mControllersToRecord = {CONTROLLER_P1_AUTO,  CONTROLLER_PLAYER_2,
                                  CONTROLLER_PLAYER_3, CONTROLLER_PLAYER_4,
                                  CONTROLLER_PLAYER_5, CONTROLLER_PLAYER_6,
                                  CONTROLLER_PLAYER_7, CONTROLLER_PLAYER_8};
}

// called if read config fails basically
void ConfigData::WriteConfig()
{
    Logger logger;
    using namespace simpleIniParser;
    std::unique_ptr<Ini> ini = std::make_unique<Ini>();

    ini->sections.push_back(new IniSection(
        SEMICOLON_COMMENT, "Which controller do we want to record?"));
    ini->sections.push_back(new IniSection(
        SEMICOLON_COMMENT,
        " they will be recorded only if they are connected/paired/on at the "
        "time user one begins recording"));

    IniSection *controllers = new IniSection(SECTION, "controllers");
    for (auto &i : mControllersToRecord) {
        controllers->options.push_back(
            new IniOption(controllerToString.at(i), "1"));
    }

    ini->sections.push_back(controllers);

    ini->sections.push_back(
        new IniSection(SEMICOLON_COMMENT, "recording options"));
    ini->sections.push_back(
        new IniSection(SEMICOLON_COMMENT, "start=immediate|input 18|time 9"));
    ini->sections.push_back(
        new IniSection(SEMICOLON_COMMENT,
                       "immediate : begin as soon as you press the shortcut"));
    ini->sections.push_back(new IniSection(
        SEMICOLON_COMMENT, "input <number> : start as soon as <number> inputs "
                           "have been pressed (think: leaving menus etc)"));
    ini->sections.push_back(new IniSection(
        SEMICOLON_COMMENT,
        "time <number> : start as soon as <number> seconds have elapsed"));
    < IniSection *record = new IniSection(SECTION, "record");

    if (std::get<0>(this->mMode) == RecordMode::IMMEDIATE) {
        record->options.push_back(new IniOption("start", "immediate"));
    } else if (std::get<0>(this->mMode) == RecordMode::INPUT_BASED) {
        std::string temp = "input " + std::get<1>(this->mMode);
        record->options.push_back(new IniOption("start", temp));
    } else if (std::get<0>(this->mMode) == RecordMode::TIME_BASED) {
        std::string temp = "time " + std::get<1>(this->mMode);
        record->options.push_back(new IniOption("start", temp));
    }

    ini->sections.push_back(record);

    if (ini->writeToFile(this->filename)) {
        logger->info("Wrote config file.\r\n");
    }
}

void ConfigData::ReadConfig()
{
    Logger logger;
    logger->info("Parsing config file.\r\n");
    bool error = false;
    using namespace simpleIniParser;

    // https://github.com/AtlasNX/SimpleIniParser/blob/master/example/Reading/source/main.cpp
    Ini *ini = Ini::parseFile(this->filename);

    // actually this std::function<bool(std::string & str)> is_only_digits =
    // []() isn't not the same type as auto feelsdankman
    // https://stackoverflow.com/a/36030880
    auto is_only_digits = [](std::string &str) {
        return str.find_first_not_of("0123456789") == std::string::npos;
    };

    try {
        IniOption *option =
            ini->findSection("record")->findFirstOption("start");

        if (option->value == "immediate") {
            logger->info("Setting recorder setting to: IMMEDIATE\r\n");
            this->SetRecordMode(std::make_tuple(RecordMode::IMMEDIATE, 0));
        } else if (size_t pos = std::string(option->value).find("input") !=
                                std::string::npos) {
            std::string temp = std::string(option->value).substr(pos);

            if (is_only_digits(temp)) {
                logger->info("Setting recorder setting to: INPUT_BASED\r\n");
                this->SetRecordMode(
                    std::make_tuple(RecordMode::INPUT_BASED, std::stoi(temp)));
            } else {
                logger->error("Error Parsing config file: Invalid input "
                              "number. Defaulting "
                              "to immediate.\r\n");
                error = true;
                this->SetRecordMode(std::make_tuple(RecordMode::IMMEDIATE, 0));
            }
        } else if (size_t pos = std::string(option->value).find("time") !=
                                std::string::npos) {
            std::string temp = std::string(option->value).substr(pos);

            if (is_only_digits(temp)) {
                logger->info("Setting recorder setting to: TIME_BASED\r\n");
                this->SetRecordMode(
                    std::make_tuple(RecordMode::TIME_BASED, std::stoi(temp)));
            } else {
                logger->error("Error Parsing config file: Invalid time "
                              "specified for time based input. Defaulting "
                              "to immediate.\r\n");
                this->SetRecordMode(std::make_tuple(RecordMode::IMMEDIATE, 0));
                error = true;
            }
        }
    } catch (...) {
        logger->error(
            "Error Parsing config file: key start not fault. Defaulting "
            "to immediate.\r\n");
        error = true;
        this->SetRecordMode(std::make_tuple(RecordMode::IMMEDIATE, 0));
    }

    IniSection *controllerOption = ini->findSection("controllers");

    this->mControllersToRecord.clear();

    if (controllerOption == nullptr) {
        logger->error(
            "Error Parsing config file controllers block not found. Defaulting "
            "to all.\r\n");
        this->mControllersToRecord = {CONTROLLER_P1_AUTO,  CONTROLLER_PLAYER_2,
                                      CONTROLLER_PLAYER_3, CONTROLLER_PLAYER_4,
                                      CONTROLLER_PLAYER_5, CONTROLLER_PLAYER_6,
                                      CONTROLLER_PLAYER_7, CONTROLLER_PLAYER_8};
    } else {

        for (auto const &i : controllerOption->options) {
            try {
                // we only check for value being 1 or true
                if (i->value == "1" || i->value == "true") {
                    this->mControllersToRecord.push_back(
                        this->stringToController.at(i->key));
                    logger->trace(
                        "Adding %s to the list of controller that are "
                        "to be recorded.\r\n",
                        i->key);
                }

            } catch (const std::out_of_range &oor) {

                logger->error("Unknown key '%s'in controller block\r\n",
                              i->key);
            }
        }
    }

    if (error) {
        logger->info("Errors in config file. Saving it anew.\r\n");
    }

    delete ini;
}
ConfigData::~ConfigData() {}