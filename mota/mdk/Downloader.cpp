#include <mota/mdk/Downloader.h>

#include <functional>

#include <stdio.h>

using namespace mota;
using namespace mota::mdk;

Downloader::Downloader()
  : state_(kInvalid)
{

}

bool Downloader::link(const string &src, const string &dst) {
  srcUrl_ = src;
  dstUrl_ = dst;
  setState(kLinked);
  return true;
}

bool Downloader::start()
{

  return true;
}

bool Downloader::stop()
{
  return true;
}

bool Downloader::pause()
{
  return true;
}