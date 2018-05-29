#ifndef MOTA_APP_DOWNLOADER_H
#define MOTA_APP_DOWNLOADER_H

#include <mota/mdk/Resolver.h>
#include <mota/mdk/Url.h>

#include <muduo/net/EventLoopThread.h>
#include <muduo/base/noncopyable.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/TcpClient.h>

#include <string>
#include <functional>
#include <memory>


namespace mota
{
namespace mdk
{
enum StartRet {kStartDnsResFailed, kStartDnsResSuccess};

typedef std::function<void(StartRet ret, unsigned short code)> StartCallBack;
typedef std::function<void()> StopCallBack;
typedef std::function<void()> PauseCallBack;
typedef std::function<void()> ResumeCallBack;


class Downloader: muduo::noncopyable {
public:
  Downloader(muduo::net::EventLoop *loop);
  ~Downloader();
  void link(const std::string &src, const std::string &dst);
  void start();
  void stop();
  void pause();
private:
  enum States { kInit, kLinked, kResolved, kConnected, kHeaderChecked, kOnTransfer, kPaused, kStopped };
  States state_;
  std::unique_ptr<muduo::net::EventLoopThread> eventLoopThread_;
  muduo::net::EventLoop *loop_;
  std::unique_ptr<muduo::net::TcpClient> client_;
  std::unique_ptr<Resolver> resolver_;
  Url srcUrl_;
  std::string dstUrl_;
  muduo::net::TcpConnectionPtr connection_;
  muduo::MutexLock mutex_;
  
  StartCallBack startCallBack_;
  StopCallBack stopCallBack_;
  PauseCallBack pauseCallBack_;
  ResumeCallBack resumeCallBack;

  void setState(States s) { state_ = s; };
  
  void connect() { client_->connect();}
  void disconnect() { client_->disconnect();}
  void onConnection(const muduo::net::TcpConnectionPtr& conn);
  void onMessage(const muduo::net::TcpConnectionPtr& conn,
				 muduo::net::Buffer* buf,
				 muduo::Timestamp receiveTime);


  
//  void setLinkCallBack(const LinkCallBack &cb){ linkCallBack_ = cb; };
  void setStartCallBack(const StartCallBack &cb){ startCallBack_ = cb; };
  void setStopCallBack(const StopCallBack &cb){ stopCallBack_ = cb; };
  void setPauseCallBack(const PauseCallBack &cb){ pauseCallBack_ = cb; };
  void setResumeCallBack(const ResumeCallBack &cb){ resumeCallBack = cb; };

  void resolveCallback(const std::string& host, const muduo::net::InetAddress &adr);

  
  
};


}
}

#endif
