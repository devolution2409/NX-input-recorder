#pragma once
#include "helpers/SystemHelper.hpp"

#include <string>

#ifndef __has_include
static_assert(false, "__has_include not supported");
#else
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif
#endif

namespace Helper {
/**
 * @brief
 *
 */

class RecordWriter {

  public:
    RecordWriter(std::string foldername);
    ~RecordWriter();
    void addToBuffer(std::string text);

  private:
    std::string buffer;

    std::string getBuffer();
    std::string mFoldername;
};

} // namespace Helper