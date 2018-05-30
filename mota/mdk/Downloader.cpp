#include <mota/mdk/HttpContext.h>
#include <mota/mdk/Downloader.h>
#include <mota/mdk/Url.h>
#include <mota/mdk/Chunk.h>

#include <muduo/base/Logging.h>
#include <muduo/base/noncopyable.h>
#include <muduo/base/StringPiece.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/EventLoop.h>
#include <muduo/base/Timestamp.h>

#include <sstream>
#include <functional>

#include <stdio.h>
#include "http_parser.h"

using muduo::Logger;
using muduo::StringPiece;
using muduo::net::Buffer;
using muduo::MutexLock;
using muduo::MutexLockGuard;
using muduo::Timestamp;

using muduo::net::EventLoop;
using muduo::net::TcpClient;
using muduo::net::EventLoopThread;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpConnection;

using namespace mota;
using namespace mota::mdk;

using std::string;

void defaultStartCallBack(StartRet ret, unsigned short code) {
}


Downloader::Downloader()
  : state_(kInit),
  eventLoopThread_(new EventLoopThread()),
  loop_(eventLoopThread_->startLoop()),
  resolver_(loop_),
  fullFilelSize_(0),
  acceptRanges_(false)
{

}

Downloader::~Downloader()
{
}

void Downloader::resolveCallback(const string& host, const InetAddress &adr)
{
  printf("host:%s => %s:%u\n", srcUrl_.host(), adr.toIp().c_str(), adr.toPort());
	if(!adr.toIp().compare("0.0.0.0")){
	  if(linkCallBack_) {
			linkCallBack_(kLinkDnsResFail, 0, 0);
	  }
	} else {
    InetAddress inetAdr(adr.toIp().c_str(), 80);
    client_.reset(new TcpClient(loop_, inetAdr, "motaClient"));
    client_->setConnectionCallback(
    std::bind(&Downloader::onConnection, this, std::placeholders::_1));
    client_->enableRetry();
    client_->setMessageCallback(
          std::bind(&Downloader::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    connect();
    setState(kResolved);
	}
}

void Downloader::link(const string &src) {
  srcUrl_ = Url(src.c_str());
  if(srcUrl_.hasHost()) {
		printf("resolve host:%s\n", srcUrl_.host());
    Resolver::Callback resolveCB = std::bind(&Downloader::resolveCallback, this, srcUrl_.host(), std::placeholders::_1);
    muduo::net::EventLoop::Functor functor = std::bind(&Resolver::resolve, &resolver_, srcUrl_.host(), resolveCB);
    loop_->runInLoop(functor);
    setState(kUrlValidated);
  } else {
    printf("no host to parse\n");
		if(linkCallBack_)
			linkCallBack_(kLinkUrlValidateFail, 0, 0);
  }
}

void Downloader::start(size_t offset)
{
  printf("start from %lu to %lu\n", offset, fullFilelSize_);
  if(offset <= fullFilelSize_) {
    std::ostringstream oss;
    oss << "GET " << srcUrl_.path();
    if(srcUrl_.hasQuery()) {
      oss <<"?";
	  oss << srcUrl_.query();
    }
	oss << " HTTP/1.1" << "\r\n";
    oss << "Host: " << srcUrl_.host() << "\r\n";
    oss << "User-Agent: mota/0.0.1" << "\r\n";
    oss << "Accept: */*\r\n";
    if(acceptRanges_) {
      oss << "Range: bytes[" << offset << "]-[" << fullFilelSize_ << "]\r\n";
    }
    oss << "\r\n";
    printf("Req:\n%s\n", oss.str().c_str());
    Buffer input;
    input.append(oss.str().c_str(), oss.str().size());
	printf("send GET req\n");
    connection_->send(input.toStringPiece().data(), static_cast<int>(input.readableBytes()));
  } else {
  }
}

void Downloader::stop()
{
}

void Downloader::pause()
{
}

void Downloader::onConnection(const TcpConnectionPtr& conn)
{
  LOG_DEBUG << conn->localAddress().toIpPort() << " -> "
           << conn->peerAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
  
  MutexLockGuard lock(mutex_);
  if (conn->connected())
  {
    setState(kConnected);
    connection_ = conn;
	
    std::ostringstream oss;
    oss << "HEAD " << srcUrl_.path();
    if(srcUrl_.hasQuery()) {
	  oss <<"?";
	  oss << srcUrl_.query();
    }
	oss << " HTTP/1.1" << "\r\n";
    oss << "Host: " << srcUrl_.host() << "\r\n";
    oss << "User-Agent: mota/0.0.1" << "\r\n";
    oss << "Accept: */*\r\n";
    oss << "\r\n";
    printf("Req:\n%s\n", oss.str().c_str());
    Buffer input;
    input.append(oss.str().c_str(), oss.str().size());
    connection_->send(input.toStringPiece().data(), static_cast<int>(input.readableBytes()));
  }
  else
  {
    connection_.reset();
  }
}

void Downloader::onMessage(const TcpConnectionPtr& conn,
			   Buffer* buf,
			   Timestamp receiveTime)
{
  HttpContext ctx;
  if(state_ == kConnected) {
    printf("Rsp:\n%s\n", buf->toStringPiece().data());
	if(ctx.parseResponse(buf, receiveTime)){
      fullFilelSize_ = ctx.response().getContentLength();
      acceptRanges_ = ctx.response().getAcceptRanges();
	  setState(kHeaderOnlyExtracted);
	  if(linkCallBack_)
	    linkCallBack_(kLinkSuccess, ctx.response().getStatusCode(), fullFilelSize_);
	}
  } else if(state_ == kHeaderOnlyExtracted) {
    if(ctx.parseResponse(buf, receiveTime)) {
	  setState(kOnBodyTransfer);
	  printf("extract GET Header successed!\n");
	  printf("%lu bytes body get\n", buf->readableBytes());
	  static size_t chunkSize = 0;
	  
	  if(ctx.response().getTransferEncoding()) {
	    chunkSize = strtol(string(buf->peek(),buf->findCRLF(buf->peek())).c_str(), nullptr, 16);
		chunk_(new Chunk(chunkSize));
		printf("chunked size:%s => %lu\n", string(buf->peek(),buf->findCRLF(buf->peek())).c_str(), chunkSize);
	  } else {
	    chunk_(new Chunk(chunkSize));
	  }
	  chunk_->append(buf->peek(), buf->readableBytes());
	  
	  buf->retrieveAll();
	  if(chunk_->isFull()) {
        if(dataCallBack_) {
          dataCallBack_(static_cast<Buffer*>(chunk_.get()), receiveTime);
		  chunk_->retrieveAll();
        }
		if(stopCallBack_) {
		  stopCallBack_();
		}
	  }
    } else {
      printf("extract GET Header failed!\n");
    }
  } else if(state_ == kOnBodyTransfer) {
    printf("%lu bytes body get\n", buf->readableBytes());
    
	buf->retrieveAll();
  }
}


