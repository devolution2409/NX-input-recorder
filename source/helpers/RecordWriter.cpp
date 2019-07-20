#include "helpers/RecordWriter.hpp"

namespace Helper {

RecordWriter::RecordWriter(std::string filename = "")
{
    if (filename.empty()) {

        u64 titleID;
        try {
            titleID = Helper::System::GetActiveTitleID();
            filename = std::to_string(titleID);
        }
        catch (...) {
            // if we couldn't get id
            filename = "Unknown.txt";
        }
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
