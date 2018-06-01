#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>

#include <mota/mdk/Downloader.h>
#include <mota/mdk/Chunk.h>

#include <memory>

#include <stdio.h>
#include <unistd.h>

using muduo::net::EventLoop;
using muduo::net::EventLoopThreadPool;
using muduo::Logger;
using muduo::net::Buffer;
using muduo::Timestamp;
using muduo::MutexLockGuard;
using muduo::MutexLock;

using mota::mdk::Downloader;
using mota::mdk::Chunk;

Downloader *d;
Chunk meta(0);

void linkCB(mota::mdk::LinkRet ret, unsigned short statusCode, size_t fileSize) {
  printf("linkCB ret:%u statusCode:%u fileSize:%lu\n", ret, statusCode, fileSize);
  if(ret == mota::mdk::kLinkSuccess) {
    d->start(0);
  }
}

void dataCB(Chunk *dst, Buffer *src, Timestamp timeStamp, MutexLock *lock) {
  {
    MutexLockGuard guard(*lock);
    dst->append(src->retrieveAllAsString());
  }
}

void stopCB(Chunk *out, mota::mdk::StopRet ret) {
  printf("stopCB ret:%u total:%lu\n", ret, out->readableBytes());
  if(ret == mota::mdk::kStopTransDone) {
    out->end();
  }
}

int main(int argc, char *argv[])
{
  Logger::setLogLevel(Logger::DEBUG);
  FILE *metaFile = fopen("update.zip", "wb");
  Chunk meta(0, metaFile);
  d = new Downloader();
//  d->link("http://ota.iservernetwork.com/files/2018/05/30/1527645595.p301-ota-20180529.zip");
//  d->link("http://ota.iservernetwork.com/files/2018/05/21/1526881922.SQLyog.zip");
  d->link("http://ota.iservernetwork.com/tvupd/metadata/fyman_demo/fyman_demo/fyman_demo/20180126/firmware_upgrade.metadata?cuid=00%3A0c%3A29%3Ae8%3Aa6%3A02&ver=&skip=");
  d->setLinkCallBack(linkCB);
  d->setDataCallBack(std::bind(dataCB, &meta, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  d->setStopCallBack(std::bind(stopCB, &meta, std::placeholders::_1));
  while(1) {
    usleep(1000*1000);
  }
  return 0;
}
