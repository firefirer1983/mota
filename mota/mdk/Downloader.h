#ifndef MOTA_APP_DOWNLOADER_H
#define MOTA_APP_DOWNLOADER_H

#include <string>
#include <functional>
#include <memory>
#include <muduo/net/TcpClient.h>

using std::string;
using muduo::net::EventLoop;
using muduo::net::TcpClient;

namespace mota
{
namespace mdk
{

typedef std::function<void()> StartCallBack;
typedef std::function<void()> StopCallBack;
typedef std::function<void()> PauseCallBack;
typedef std::function<void()> ResumeCallBack;


class Downloader {
public:
  Downloader();
  ~Downloader();
  bool link(const string &src, const string &dst);
  bool start();
  bool stop();
  bool pause();

private:
  enum States { kInvalid, kLinked, kStopped, kPaused, kDownloading };
  shared_ptr<EventLoop> eventLoopSptr_;
  shared_ptr<TcpClient> clientSptr_;
  string srcUrl_;
  string dstUrl_;
  States state_;

  StartCallBack startCallBack_;
  StopCallBack stopCallBack_;
  PauseCallBack pauseCallBack_;
  ResumeCallBack resumeCallBack;

  void setState(States s) { state_ = s; };
  void setStartCallBack(const StartCallBack &cb){ startCallBack_ = cb; };
  void setStopCallBack(const StopCallBack &cb){ stopCallBack_ = cb; };
  void setPauseCallBack(const PauseCallBack &cb){ pauseCallBack_ = cb; };
  void setResumeCallBack(const ResumeCallBack &cb){ resumeCallBack = cb; };
};


}
}

#endif