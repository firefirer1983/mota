#include <mota/mdk/DataFilter.h>

#include <muduo/net/Buffer.h>

#include <string>

using namespace mota;
using namespace mota::mdk;

using muduo::net::Buffer;

using std::string;

bool DataFilter::header(Buffer *buf, Timestamp timeStamp)
{
  bool ok = ctx_.parseResponse(buf, timeStamp);
  if(ok){
    contentLength_ = ctx_.response().getContentLength();
    acceptRange_ = ctx_.response().getAcceptRanges();
    statusCode_ = ctx_.response().getStatusCode();
    ctx_.reset();
  }
  return ok;
}

size_t DataFilter::body(Buffer *buf, Timestamp timeStamp) {
  static bool headerReceived = false;
  size_t received = 0;
  if(headerReceived || ctx_.parseResponse(buf, timeStamp)) {
    if(!headerReceived) {
      headerReceived = true;
      transferEncoding_ = ctx_.response().getTransferEncoding();
      
      if(transferEncoding_) {
        const char *crlf = buf->findCRLF(buf->peek());
        contentLength_ = strtol(string(buf->peek(), crlf).c_str(), nullptr, 16);
        printf("chunked size: %lu\n", contentLength_);
        buf->retrieveUntil(crlf+2);
      } 
      if(buf->readableBytes() >= contentLength_) {
        partialLength_ += contentLength_;
        received = contentLength_;
      } else {
        partialLength_ += buf->readableBytes();
        received = buf->readableBytes();
      }
      ctx_.reset();
    } else {
      if(partialLength_ >= contentLength_) {
        received = 0;
      } else if(partialLength_ + buf->readableBytes() >= contentLength_) {
        received = contentLength_ - partialLength_;
      } else {
        partialLength_ += buf->readableBytes();
        received = buf->readableBytes();
      }
    }
  } else {
    printf("GET header on going!\n");
  }
  return received;
}

