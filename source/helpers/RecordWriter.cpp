#include "helpers/RecordWriter.hpp"

namespace Helper {

RecordWriter::RecordWriter(std::string foldername)
{
    std::fstream test;

    test.open("/input-recorder/debug_log2.txt", std::fstream::out);
    test << "hello from constructor" << std::endl;

    // setting up the folder in which we want to write the recording
    int i = 0;
    test << ++i << std::endl;
    if (foldername.empty()) {
        test << ++i << std::endl;
        u64 titleID;
        try {
            titleID = Helper::System::GetActiveTitleID();
            foldername = std::to_string(titleID);
        }
        catch (...) {
            // if we couldn't get id
            test << ++i << "catch block detected" << std::endl;
            foldername = "unknown";
        }
    }
    test << ++i << std::endl;
    // in any case we check if the directory exists
    filesystem::directory_entry dir(filesystem::path("sdcard:/input-recorder"));
    // if input-recorder exist
    test << ++i << std::endl;
    if (dir.exists()) {
        test << ++i << "dir exists" << std::endl;
        dir = filesystem::directory_entry(
            filesystem::path("sdcard:/input-recorder/" + foldername));
        // check if sub directory exists
        if (dir.exists()) {
            test << ++i << "this one should exist" << std::endl;
            this->mFoldername = dir.path();
        }
        test << ++i << "trying to create dir" << std::endl;
        filesystem::create_directory(this->mFoldername);
    }
    else {
        filesystem::create_directory(
            filesystem::path("sdcard:/input-recorder"));
        filesystem::create_directory(
            filesystem::path("sdcard:/input-recorder/" + foldername));
    }

    if (R_FAILED(timeInitialize())) {
        // random file name i guess
    }
    else {
        u64 now;
        timeGetCurrentTime(TimeType_UserSystemClock, &now);
        this->mFilename =
            this->mFoldername.string() + "/" + std::to_string(now) + ".record";
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
