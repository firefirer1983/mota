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

using mota::mdk::Downloader;

int main(int argc, char *argv[])
{
  Logger::setLogLevel(Logger::DEBUG);
  Downloader d;
  d.link("http://ota.iservernetwork.com/files/2018/05/19/1526696252.update.zip", "indxe.html");
	while(1) {
	  usleep(1000*1000);
	}
  return 0;
}
