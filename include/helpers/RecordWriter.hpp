#pragma once
#include "helpers/SystemHelper.hpp"
#include <string>

namespace Helper {
/**
 * @brief
 *
 */

class RecordWriter {

public:
  RecordWriter(std::string filename);
  ~RecordWriter();
  void addToBuffer(std::string text);

private:
  std::string buffer;

  std::string getBuffer();
  std::string mFilename;
};

} // namespace Helper