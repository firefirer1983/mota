#ifndef MOTA_MDK_DATAFILTER_H
#define MOTA_MDK_DATAFILTER_H

#include <mota/mdk/HttpContext.h>

#include <muduo/net/Buffer.h>

namespace mota
{
namespace mdk
{

class DataFilter
{
public:
	DataFilter():
    hasHeader_(false),
    contentLength_(0),
    partialLength_(0),
    statusCode_(0),
    acceptRange_(false),
    transferEncoding_(false){};
    
	~DataFilter(){};
	bool header(muduo::net::Buffer *buf, muduo::Timestamp timeStamp);
	size_t body(muduo::net::Buffer *buf, muduo::Timestamp timeStamp);
  unsigned short getStatusCode() { return statusCode_; }
  size_t getContentLength() { return contentLength_; }
  bool isFinished() { return contentLength_&&(partialLength_ == contentLength_); }
  bool hasHeader(){ return hasHeader_; };
  void reset();
private:
  bool hasHeader_;
  size_t contentLength_;
  size_t partialLength_;
  unsigned short statusCode_;
  bool acceptRange_;
  bool transferEncoding_;
  HttpContext ctx_;
};

}
}

#endif
