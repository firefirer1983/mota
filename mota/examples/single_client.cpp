#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/base/Logging.h>


#include <mota/mdk/Downloader.h>

#include <stdio.h>

using muduo::net::EventLoop;
using muduo::net::EventLoopThreadPool;
using muduo::Logger;

using mota::mdk::Downloader;

const int kThreadNum = 10;

void poolInitCB(EventLoop *loop) {
}

int main(int argc, char *argv[])
{
  Logger::setLogLevel(Logger::DEBUG);
  EventLoop loop;
  EventLoopThreadPool pool(&loop, "threadPool");
  pool.setThreadNum(kThreadNum);
  pool.start(poolInitCB);
  Downloader d(pool.getNextLoop());
  d.link("http://ota.iservernetwork.com/files/2018/05/19/1526696252.update.zip", "indxe.html");
  loop.loop();
  return 0;
}
