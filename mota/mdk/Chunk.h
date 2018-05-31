#ifndef MOTA_MDK_CHUNK_H
#define MOTA_MDK_CHUNK_H

#include <muduo/net/Buffer.h>
namespace mota
{
namespace mdk
{

class Chunk: public muduo::net::Buffer
{
public:
  Chunk(size_t size = 0, FILE *fp = nullptr):
    muduo::net::Buffer(),
		chunkSize_(size),
		writeFp_(fp),
    writeWaterMark_(kDefaultWaterMark){}
    
  ~Chunk() {}
	
  void append(const muduo::StringPiece& str) {
    append(str.data(), str.size());
  }
  
  void append(const char* /*restrict*/ data, size_t len) {
    if(chunkSize_&&(chunkSize_ < readableBytes() + len)) { len = chunkSize_- readableBytes(); }
    if(len) { muduo::net::Buffer::append(static_cast<const char*>(data), len); }
    if(writeFp_&&(readableBytes() >= writeWaterMark_)) {
      fwrite(peek(), writeWaterMark_, 1, writeFp_);
      retrieve(writeWaterMark_);
    }
  }
	
  void append(const void* /*restrict*/ data, size_t len) {
    if(chunkSize_&&(chunkSize_ < readableBytes() + len)) { len = chunkSize_- readableBytes(); }
    if(len) { muduo::net::Buffer::append(static_cast<const char*>(data), len); }
    if(writeFp_&&(readableBytes() >= writeWaterMark_)) {
      fwrite(peek(), writeWaterMark_, 1, writeFp_);
      retrieve(writeWaterMark_);
    }
  }
  
  void prepend(const void* /*restrict*/ data, size_t len) = delete;
	
  void prependInt64(int64_t x) = delete;
  
  void prependInt32(int32_t x) = delete;

  void prependInt16(int16_t x) = delete;

  void prependInt8(int8_t x) = delete;

  bool isFull() { return (readableBytes() == chunkSize_); }
  
	void swap(Chunk &that) {
	  if(this != &that) {
			std::swap(chunkSize_, that.chunkSize_);
			dynamic_cast<muduo::net::Buffer*>(this)->swap(*dynamic_cast<muduo::net::Buffer*>(&that));
	  }
	}

  void end() {
    if(writeFp_) {
      if(readableBytes()) {
        fwrite(peek(), 1, readableBytes(), writeFp_);
      }
      fflush(writeFp_);
      fclose(writeFp_);
    }
    retrieveAll();
  }

private:
  size_t chunkSize_;
  FILE *writeFp_;
  size_t writeWaterMark_;
  static const size_t kDefaultWaterMark = 16*1024;
};


}
}
#endif
