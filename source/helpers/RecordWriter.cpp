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
            test << std::endl << "titleID:" << titleID << std::endl;
            this->mFoldername = std::to_string(titleID);
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
        // random file name i guess
    }
    else {

        // store a lambda

        std::function<std::string()> getHumanReadableTime = []() {
            u64 now;
            timeGetCurrentTime(TimeType_UserSystemClock, &now);
            const time_t rawtime = static_cast<const time_t>(now);
            struct tm *dt;
            char buffer[30];
            dt = localtime(&rawtime);
            strftime(buffer, sizeof(buffer), "%a-%b-%d-%T", dt);
            return std::string(buffer);
        };

        this->mFilename = this->mFoldername.string() + "/" +
                          getHumanReadableTime() + ".record";
        timeExit();
    }
    fs.open(this->mFilename, std::fstream::out);
    test.close();
}

RecordWriter::~RecordWriter() { fs.close(); }

void RecordWriter::AddInputs(InputInfos info) { this->infos.push_back(info); }

void RecordWriter::WriteInfos()
{
    for (const auto &i : this->infos) {
        fs << i.controller << " " << i.kHeld << " " << i.lPos.dx << " "
           << i.lPos.dy << " " << i.rPos.dx << " " << i.rPos.dy << " ";
    }
    fs << std::endl;
}

} // namespace Helper
