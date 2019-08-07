#include "helpers/RecordWriter.hpp"

namespace Helper
{

RecordWriter::RecordWriter(std::string foldername)
{
    Logger logger;
    logger->trace("Created Writer\n");

    filesystem::path basePath = "/input-recorder";

    // setting up the folder in which we want to write the recording
    if (foldername.empty()) {
        u64 titleID;
        try {
            titleID = Helper::System::GetActiveTitleID();
            // convert to decimal representation feelsdankman
            std::stringstream ss; // heil forsenSheffy
            ss << std::hex << titleID;
            // prefixing 0 and setting everything uppercase
            std::string temp = "0" + ss.str();
            for (auto &c : temp)
                c = toupper(c);

            this->mFoldername = temp;
        } catch (...) {
            // if we couldn't get id
            this->mFoldername = "unknown";
        }
    }

    filesystem::path fullPath =
        basePath.string() + "/" + this->mFoldername.string();

    if (filesystem::exists(basePath)) {

        // check if sub directory exists
        if (!filesystem::exists(fullPath)) {
            filesystem::create_directory(fullPath);
        }
        this->mFoldername = fullPath;
    } else {
        filesystem::create_directory(basePath);

        filesystem::create_directory(fullPath);
    }

    if (R_FAILED(timeInitialize())) {
        logger->error(
            "Failed to initialize time library. Filename will be random.\r\n");
        // random file name i guess
    } else {

        // store a lambda
        // also works with std::function<std::string()> type
        auto getHumanReadableTime = [&]() {
            u64 now;
            timeGetCurrentTime(TimeType_UserSystemClock, &now);
            const time_t rawtime = static_cast<const time_t>(now);

            std::string hrTime = strtok(ctime(&rawtime), "\n");
            for (auto &c : hrTime) {
                if (c == ' ')
                    c = '_';
                else if (c == ':')
                    c = '-';
            }

            return hrTime;
        };

        this->mFilename = this->mFoldername.string() + "/" +
                          getHumanReadableTime() + ".record";
        timeExit();
    }
    logger->trace("Filename is: '%s'\r\n", this->mFilename.c_str());
    this->mFs.open(this->mFilename, std::fstream::out);
}

RecordWriter::~RecordWriter()
{
    // don't need to close the file as destructor of fstream does that..
}

void RecordWriter::AddInputs(InputInfos info) { this->infos.push_back(info); }

void RecordWriter::WriteInfos()
{
    for (const auto &i : this->infos) {
        this->mFs << i.controller << " " << i.kHeld << " " << i.lPos.dx << " "
                  << i.lPos.dy << " " << i.rPos.dx << " " << i.rPos.dy << " "
                  << i.motions.accelerometer.x << " "
                  << i.motions.accelerometer.y << " "
                  << i.motions.accelerometer.z << " " << i.motions.gyroscope.x
                  << " " << i.motions.gyroscope.y << " "
                  << i.motions.gyroscope.z << " " << i.motions.unk.x << " "
                  << i.motions.unk.y << " " << i.motions.unk.z << " "
                  << i.motions.orientation[0].x << " "
                  << i.motions.orientation[0].y << " "
                  << i.motions.orientation[0].z << " "
                  << i.motions.orientation[1].x << " "
                  << i.motions.orientation[1].y << " "
                  << i.motions.orientation[1].z << " "
                  << i.motions.orientation[2].x << " "
                  << i.motions.orientation[2].y << " "
                  << i.motions.orientation[2].z << " ";
    }
    this->mFs << std::endl;
    // don't forget to clear the infos to prevent retarded behavior OMGScoods
    this->infos.clear();
}

} // namespace Helper
