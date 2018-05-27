#include <mota/mdk/Downloader.h>
#include <muduo/base/noncopyable.h>

#include <functional>

#include <stdio.h>

using namespace mota;
using namespace mota::mdk;

Downloader::Downloader()
  : state_(kInvalid),
  eventLoopThread_(new EventLoopThread),
  loop_(eventLoopThread_->startLoop()),
  resolver_(new mota::mdk::Resolver(loop_))
{

}

Downloader::~Downloader()
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
