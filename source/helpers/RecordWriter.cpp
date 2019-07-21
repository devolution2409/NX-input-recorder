#include "helpers/RecordWriter.hpp"

namespace Helper {

RecordWriter::RecordWriter(std::string foldername)
{
    std::fstream test;
    filesystem::path basePath = "/input-recorder";

    test.open("/input-recorder/debug_log2.txt", std::fstream::out);
    test << "hello from constructor" << std::endl;

    // setting up the folder in which we want to write the recording
    test << "here" << std::endl;
    if (foldername.empty()) {
        test << "nig" << std::endl;
        u64 titleID;
        try {
            titleID = Helper::System::GetActiveTitleID();
            // convert to decimal representation feelsdankman
            std::stringstream ss; // heil forsenSheffy
            ss << std::hex << titleID;

            test << std::endl << "titleID:" << std::hex << titleID << std::endl;
            // prefixing 0 and setting everything uppercase
            std::string temp = "0" + ss.str();
            for (auto &c : temp)
                c = toupper(c);

            this->mFoldername = temp;
        }
        catch (...) {
            // if we couldn't get id
            test << "catch block detected" << std::endl;
            this->mFoldername = "unknown";
        }
    }

    filesystem::path fullPath =
        basePath.string() + "/" + this->mFoldername.string();

    if (filesystem::exists(basePath)) {
        test << basePath << " dir exists" << std::endl;
        // check if sub directory exists
        if (filesystem::exists(fullPath)) {
            test << "this one exists for some reason:" << fullPath << std::endl;
        }
        else {
            test << "trying to create dir" << std::endl;
            filesystem::create_directory(fullPath);
        }
        this->mFoldername = fullPath;
    }
    else {
        filesystem::create_directory(basePath);

        filesystem::create_directory(fullPath);
    }

    if (R_FAILED(timeInitialize())) {
        test << "failed to initialize time forsenD" << std::endl;
        // random file name i guess
    }
    else {

        // store a lambda

        std::function<std::string()> getHumanReadableTime = [&]() {
            u64 now;
            timeGetCurrentTime(TimeType_UserSystemClock, &now);
            const time_t rawtime = static_cast<const time_t>(now);
            test << "raw time: " << rawtime << std::endl;
            // this doesnt seem to work
            /*
            struct tm *dt;
            dt = localtime(&rawtime);
            test << "struct time: " << asctime(dt) << std::endl;
            char buffer[30];
            strftime(buffer, sizeof(buffer), "%a-%b-%d-%T", dt); */

            std::string hrTime = strtok(ctime(&rawtime), "\n");
            for (auto &c : hrTime) {
                if (c == ' ')
                    c = '_';
                else if (c == ':')
                    c = '-';
            }

            test << std::endl << "hrtime is now:" << hrTime << std::endl;

            return hrTime;
        };

        this->mFilename = this->mFoldername.string() + "/" +
                          getHumanReadableTime() + ".record";
        timeExit();
    }

    test << std::endl << "trying Touching file:" << this->mFilename;

    this->mFs.open(this->mFilename, std::fstream::out);
    this->mFs << "hi nibs";
    test.close();
}

RecordWriter::~RecordWriter() { this->mFs.close(); }

void RecordWriter::AddInputs(InputInfos info) { this->infos.push_back(info); }

void RecordWriter::WriteInfos()
{
    for (const auto &i : this->infos) {
        this->mFs << i.controller << " " << i.kHeld << " " << i.lPos.dx << " "
                  << i.lPos.dy << " " << i.rPos.dx << " " << i.rPos.dy << " ";
    }
    this->mFs << std::endl;
}

} // namespace Helper
