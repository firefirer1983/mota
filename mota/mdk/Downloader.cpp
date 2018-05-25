#include <mota/mdk/Downloader.h>

#include <functional>

#include <stdio.h>

using namespace mota;
using namespace mota::mdk;

Downloader::Downloader()
  : state_(kStopped)
{

}

bool Downloader::start(const string &src, const string &dst)
{
	srcUrl_ = src;
	dstUrl_ = dst;
	return true;
}