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
  Chunk(size_t size):
    chunkSize_(size),
	Buffer(size)
  {
  }
  ~Chunk() {};
	
  void append(const muduo::::StringPiece& str)
  {
    append(str.data(), str.size());
  }
  
  void append(const void* /*restrict*/ data, size_t len)
  {
    if(chunkSize_ < (readableBytes() + len)) {
      len = chunkSize_- readableBytes();
    }
	if(len) {
      muduo::net::Buffer::append(static_cast<const char*>(data), len);
	}
  }
  
  void prepend(const void* /*restrict*/ data, size_t len)
  {
    if(chunkSize_ < readableBytes() + len) {
	  len = chunkSize_ - readableBytes();
    }
	if(len) {
	  muduo::net::Buffer::prepend(data, len);
	}
  }
  bool isFull() {
    return (readableBytes() == chunkSize_);
  }
private:
  size_t chunkSize_;
};


}
}
#endif
