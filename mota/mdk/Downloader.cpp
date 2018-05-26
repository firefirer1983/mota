#include <mota/mdk/Downloader.h>

#include <functional>

#include <stdio.h>

using namespace mota;
using namespace mota::mdk;

Downloader::Downloader()
  : state_(kInvalid),
  eventLoopThread_(new EventLoopThread),
  loop_(eventLoopSptr_.startLoop())
{

}

void Downloader::link(const string &src, const string &dst) {
  srcUrl_ = src;
  dstUrl_ = dst;
}

void Downloader::start()
{
}

void Downloader::stop()
{
}

void Downloader::pause()
{
}