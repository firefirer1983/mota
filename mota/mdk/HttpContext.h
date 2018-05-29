// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MOTA_MDK_HTTPCONTEXT_H
#define MOTA_MDK_HTTPCONTEXT_H

#include <muduo/base/copyable.h>
#include <muduo/net/Buffer.h>

#include <mota/mdk/HttpRequest.h>
#include <mota/mdk/HttpResponse.h>


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



class HttpContext : public muduo::copyable
{
 public:
  enum HttpRequestParseState
  {
    kExpectRequestLine,
    kExpectHeaders,
    kExpectBody,
    kGotAll,
  };
	
  enum HttpResponseParseState
  {
    kExpectResponseLine,
    kExpectResponseHeaders,
    kExpectResponseBody,
    kGotAllResponse,
  };
  
  HttpContext()
    : reqState_(kExpectRequestLine),
	  rspState_(kExpectResponseLine)
  {
  }
		
  // default copy-ctor, dtor and assignment are fine
  // return false if any error
  bool parseRequest(muduo::net::Buffer* buf, Timestamp receiveTime);
  bool parseResponse(muduo::net::Buffer* buf, Timestamp receiveTime);
  // bool expectRequestLine() const
  // { return state_ == kExpectRequestLine; }

  // bool expectHeaders() const
  // { return state_ == kExpectHeaders; }

  // bool expectBody() const
  // { return state_ == kExpectBody; }

  bool gotRspAll() const
  { return rspState_ == kGotAllResponse; }
	
  bool gotReqAll() const
  { return reqState_ == kGotAll; }

  void reset()
  {
		reqState_ = kExpectRequestLine;
    rspState_ = kExpectResponseLine;
    {
			HttpResponse dummy;
      response_.swap(dummy);
    }
    {
		  HttpRequest dummy;
      request_.swap(dummy);
    }	
  }

  const HttpRequest& request() const
  { return request_; }

  HttpRequest& request()
  { return request_; }
  
  const HttpResponse& response() const
  { return response_; }

  HttpResponse& response()
  { return response_; }

 private:
  bool processRequestLine(const char* begin, const char* end);
  bool processResponseLine(const char* begin, const char* end);
  HttpRequestParseState reqState_;
  HttpResponseParseState rspState_;
  HttpRequest request_;
  HttpResponse response_;
};

}
}

#endif  // MUDUO_NET_HTTP_HTTPCONTEXT_H
