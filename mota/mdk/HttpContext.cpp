// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include <muduo/net/Buffer.h>
#include <mota/mdk/HttpContext.h>

using muduo::net::Buffer;

using namespace mota;
using namespace mota::mdk;

using std::string;

bool HttpContext::processRequestLine(const char* begin, const char* end)
{
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  if (space != end && request_.setMethod(start, space))
  {
    start = space+1;
    space = std::find(start, end, ' ');
    if (space != end)
    {
      const char* question = std::find(start, space, '?');
      if (question != space)
      {
        request_.setPath(start, question);
        request_.setQuery(question, space);
      }
      else
      {
        request_.setPath(start, space);
      }
      start = space+1;
      succeed = end-start == 8 && std::equal(start, end-1, "HTTP/1.");
      if (succeed)
      {
        if (*(end-1) == '1')
        {
          request_.setVersion(HttpRequest::kHttp11);
        }
        else if (*(end-1) == '0')
        {
          request_.setVersion(HttpRequest::kHttp10);
        }
        else
        {
          succeed = false;
        }
      }
    }
  }
  return succeed;
}

// return false if any error
bool HttpContext::parseRequest(Buffer* buf, Timestamp receiveTime)
{
  bool ok = true;
  bool hasMore = true;
  while (hasMore)
  {
    if (reqState_ == kExpectRequestLine)
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        ok = processRequestLine(buf->peek(), crlf);
        if (ok)
        {
          request_.setReceiveTime(receiveTime);
          buf->retrieveUntil(crlf + 2);
          reqState_ = kExpectHeaders;
        }
        else
        {
          hasMore = false;
        }
      }
      else
      {
        hasMore = false;
      }
    }
    else if (reqState_ == kExpectHeaders)
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        const char* colon = std::find(buf->peek(), crlf, ':');
        if (colon != crlf)
        {
          request_.addHeader(buf->peek(), colon, crlf);
        }
        else
        {
          // empty line, end of header
          // FIXME:
          reqState_ = kGotAll;
          hasMore = false;
        }
        buf->retrieveUntil(crlf + 2);
      }
      else
      {
        hasMore = false;
      }
    }
    else if (reqState_ == kExpectBody)
    {
      // FIXME:
    }
  }
  return ok;
}

bool HttpContext::processResponseLine(const char* begin, const char* end)
{
  bool succeed = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  if (space != end && response_.setStatusCode(space+1, end))
  {
    response_.setVersion(*(space-1) == '0'?HttpResponse::kHttp10:*(space-1) == '1'?HttpResponse::kHttp11:HttpResponse::kUnknowVersion);
    succeed = true;
  }
  return succeed;
}


bool HttpContext::parseResponse(Buffer* buf, Timestamp receiveTime)
{
  bool ok = true;
  bool hasMore = true;
  while (hasMore)
  {
    if (rspState_ == kExpectResponseLine)
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        ok = processResponseLine(buf->peek(), crlf);
        if (ok)
        {
//          response_.setReceiveTime(receiveTime);
          buf->retrieveUntil(crlf + 2);
          rspState_ = kExpectResponseHeaders;
        }
        else
        {
          hasMore = false;
        }
      }
      else
      {
        hasMore = false;
      }
    }
    else if (rspState_ == kExpectResponseHeaders)
    {
      const char* crlf = buf->findCRLF();
      if (crlf)
      {
        const char* colon = std::find(buf->peek(), crlf, ':');
        if (colon != crlf)
        {
          response_.addHeader(buf->peek(), colon, crlf);
        }
        else
        {
          // empty line, end of header
          // FIXME:
//          printf("empty line\n");
          rspState_ = kGotAllResponse;
          hasMore = false;
        }
        buf->retrieveUntil(crlf + 2);
      }
      else
      {
        hasMore = false;
      }
    }
    else if (rspState_ == kExpectResponseBody)
    {
      // FIXME:
    }
  }
  return ok;
}

