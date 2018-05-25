#ifndef MOTA_APP_DOWNLOADER_H
#define MOTA_APP_DOWNLOADER_H

#include <string>
using std::string;

namespace mota
{
namespace mdk
{

class Downloader {
public:
  Downloader();
  ~Downloader();
  bool start(const string &src, const string &dst);
  bool stop();
  bool pause();

private:
  enum States { kStopped, kPaused, kDownloading };
  string srcUrl_;
  string dstUrl_;
  States state_;
  void setState(States s) { state_ = s; };
};

}
}

#endif