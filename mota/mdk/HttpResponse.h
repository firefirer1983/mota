// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MOTA_MDK_HTTPRESPONSE_H
#define MOTA_MDK_HTTPRESPONSE_H

#include <muduo/base/copyable.h>
#include <muduo/base/Types.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/Timestamp.h>

#include <map>
namespace muduo
{
namespace net
{
class Buffer;
}
}

namespace mota
{
namespace mdk
{


class HttpResponse : public muduo::copyable
{
 public:
  enum Version
  {
    kUnknowVersion, kHttp10, kHttp11
  };
 	
  void setVersion(Version v) { version_ = v; }
  Version getVersion() const { return version_; }
  
  enum HttpStatusCode
  {
    kUnknown,
    k100_Contiue = 100,
    k101_Swiching_Protocols = 101,
    k200_OK = 200,
    k301_Moved_Permanently = 301,
    k400_Bad_Request = 400,
    k404_Not_Found = 404,
  };
	
  std::map<std::string, int> StatusCodeMap{
  	{"100 Continue", 100},
  	{"101 Switching Protocols", 101},
  	{"200 OK", 200},
  	{"201 Created", 201},
  	{"206 Partial Content", 206},
  	{"301 Moved Permanently", 301},
    {"400 Bad Request", 400},
    {"404 Not Found", 404}
  };
  explicit HttpResponse(bool close)
    : statusCode_(kUnknown),
      closeConnection_(close)
  {
  }
  explicit HttpResponse()
	: statusCode_(kUnknown),
	  closeConnection_(false)
  {
  }

  bool setStatusCode(const char* start, const char* end)
  { 
    std::string c(start, end);
    printf("setStatusCode:[%s]\n", c.c_str());
    auto it = StatusCodeMap.find(c);
    if(it == StatusCodeMap.end()) {
      return false;
    } else {
      statusCode_ = static_cast<HttpStatusCode>(it->second);
      return true;
    }
  }

  unsigned short getStatusCode() {
    return static_cast<unsigned short>(statusCode_);
  }
  void setStatusMessage(const std::string& message)
  { statusMessage_ = message; }

  void setCloseConnection(bool on)
  { closeConnection_ = on; }

  bool closeConnection() const
  { return closeConnection_; }

  size_t getContentLength() const
  {
    size_t len = 0;
    auto retItor = headers_.find("Content-Length");
    if(retItor != headers_.end()) {
      len = static_cast<size_t>(atoi(retItor->second.c_str()));
    }
    return len;
  }
	
  bool getAcceptRanges() const
  {
    bool range = false;
    auto retItor = headers_.find("Accept-Ranges");
    if(retItor != headers_.end()) {
      range = !(retItor->second.compare("bytes"));
    }
    return range;
  }

  bool getTransferEncoding() const
  {
    bool chunked = false;
	auto retItor = headers_.find("Transfer-Encoding");
	if(retItor != headers_.end()) {
      chunked = !(retItor->second.compare("chunked"));
    }
    return chunked;
  }
  
  void addHeader(const char* start, const char* colon, const char* end)
  {
    std::string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
    {
      ++colon;
    }
    std::string value(colon, end);
    while (!value.empty() && isspace(value[value.size()-1]))
    {
      value.resize(value.size()-1);
    }
    headers_[field] = value;
//		printf("addHeader[%s:%s]\n",field.c_str(), value.c_str());
  }

  void setBody(const std::string& body)
  { body_ = body; }
  
  void setReceiveTime(muduo::Timestamp t)
  { receiveTime_ = t; }

  void appendToBuffer(muduo::net::Buffer* output) const;
  
  void swap(HttpResponse& that)
  {
    std::swap(statusCode_, that.statusCode_);
    std::swap(version_, that.version_);
    statusMessage_.swap(that.statusMessage_);
    std::swap(closeConnection_, that.closeConnection_);
    body_.swap(that.body_);
    headers_.swap(that.headers_);
  }

 private:
  std::map<std::string, std::string> headers_;
  HttpStatusCode statusCode_;
  Version version_;
  // FIXME: add http version
  std::string statusMessage_;
  bool closeConnection_;
  muduo::Timestamp receiveTime_;
  std::string body_;
};

}
}

#endif  // MUDUO_NET_HTTP_HTTPRESPONSE_H
