#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/base/Logging.h>


#include <mota/mdk/Downloader.h>

#include <stdio.h>
#include <unistd.h>

using muduo::net::EventLoop;
using muduo::net::EventLoopThreadPool;
using muduo::Logger;
using muduo::net::Buffer;
using muduo::Timestamp;

using mota::mdk::Downloader;
Downloader *d;

void linkCB(mota::mdk::LinkRet ret, unsigned short statusCode, size_t fileSize) {
  printf("linkCB ret:%u statusCode:%u fileSize:%lu\n", ret, statusCode, fileSize);
  if(ret == mota::mdk::kLinkSuccess) {
    d->start(0);
  }
}

void dataCB(Buffer *buf, Timestamp timeStamp) {
  printf("recv ==>%lu bytes\n", buf->readableBytes());
}

void stopCB() {
}

int main(int argc, char *argv[])
{
  Logger::setLogLevel(Logger::DEBUG);
  Buffer metaBuf;
  d = new Downloader();
  
  d->link("http://ota.iservernetwork.com/files/2018/05/30/1527645595.p301-ota-20180529.zip");
//  d->link("http://ota.iservernetwork.com/tvupd/metadata/fyman_demo/fyman_demo/fyman_demo/20180126/firmware_upgrade.metadata?cuid=00%3A0c%3A29%3Ae8%3Aa6%3A02&ver=&skip=");
  d->setLinkCallBack(linkCB);
  d->setDataCallBack(dataCB);
  while(1) {
    usleep(1000*1000);
  }
  return 0;
}
