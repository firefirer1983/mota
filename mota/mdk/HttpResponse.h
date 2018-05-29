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
	  printf("set status code failed!\n");
	  return false;
	} else {
	  printf("set status code success!\n");
      statusCode_ = static_cast<HttpStatusCode>(it->second);
	  return true;
	}
  }

  void setStatusMessage(const std::string& message)
  { statusMessage_ = message; }

  void setCloseConnection(bool on)
  { closeConnection_ = on; }

  bool closeConnection() const
  { return closeConnection_; }

  void setContentType(const std::string& contentType)
  { addHeader("Content-Type", contentType); }

  // FIXME: replace std::string with StringPiece
  void addHeader(const std::string& key, const std::string& value)
  { headers_[key] = value; }

  void setBody(const std::string& body)
  { body_ = body; }

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
  std::string body_;
};

}
}

#endif  // MUDUO_NET_HTTP_HTTPRESPONSE_H
