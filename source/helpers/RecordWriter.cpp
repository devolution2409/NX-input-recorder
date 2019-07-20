#include "helpers/RecordWriter.hpp"

namespace Helper {

RecordWriter::RecordWriter(std::string foldername = "")
{
    if (foldername.empty()) {

        u64 titleID;
        try {
            titleID = Helper::System::GetActiveTitleID();
            foldername = std::to_string(titleID);
        }
        catch (...) {
            // if we couldn't get id
            foldername = "unknown";
        }
    }
    // in any case we check if the directory exists
    std::filesystem::directory dir = "sdcard:/input-recorder";

    if (dir.exists()) {
        dir = "sdcard:/input-recorder/" + foldername;
        if (dir.exists()) {
            this->mFoldername = dir;
        }
        std::filesystem::create_directory(
            std::filesystem::path(this->mFoldername));
    }
}

std::string RecordWriter::getBuffer() { return this->buffer; }

RecordWriter::~RecordWriter() {}

void RecordWriter::addToBuffer(std::string text)
{
    if (this->buffer.empty()) {
        this->buffer = text;
    }
    else {
        this->buffer += text;
    }
}

} // namespace Helper
