#pragma once
#include "helpers/SystemHelper.hpp"

// need the struct
#include "Recorder.hpp"

#include <ctime> ///strftime
#include <fstream>
#include <functional>
#include <string>

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

namespace Helper {

class RecordWriter {

  public:
    RecordWriter(std::string foldername = "");
    ~RecordWriter();

    void AddInputs(InputInfos);
    void WriteInfos();

  private:
    std::string buffer;

    filesystem::path mFoldername;
    std::string mFilename;
    std::fstream fs;

    std::vector<InputInfos> infos;
};

} // namespace Helper