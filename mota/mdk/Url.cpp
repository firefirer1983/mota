#include <mota/mdk/Url.h>

#include "http_parser.h"

#include <vector>

#include <stdio.h>
using namespace mota;
using namespace mota::mdk;
using std::string;
using std::vector;

Url::Url(const char *url):
	url_(url)
{
  vector<string>().swap(parsed_);
  parsed_.reserve(UF_MAX-1);
  struct http_parser_url u;
  http_parser_url_init(&u);
  http_parser_parse_url(url_.data(), url_.size(), 0, &u);
  fieldset_ = std::bitset<16>(u.field_set);
  for(int i=UF_SCHEMA; i<UF_MAX; i++) {
    if(fieldset_.test(i)) {
	  parsed_.emplace_back((url_.data()+u.field_data[i].off), u.field_data[i].len);
	  printf("%s\n", parsed_.back().c_str());
    } else {
      parsed_.emplace_back("");
    }
  }
}

const char *Url::schema() 
{
  return parsed_[UF_SCHEMA].c_str();
}

const char *Url::host() 
{
  return parsed_[UF_HOST].c_str();
}

unsigned short Url::port()
{
  return atoi(parsed_[UF_PORT].c_str());
}

const char *Url::path() 
{
  return parsed_[UF_PATH].c_str();
}

const char *Url::query() 
{
  return parsed_[UF_QUERY].c_str();
}

const char *Url::fragment() 
{
  return parsed_[UF_FRAGMENT].c_str();
}

const char *Url::userinfo() 
{
  return parsed_[UF_USERINFO].c_str();
}

bool Url::hasSchema()
{
  return fieldset_.test(UF_SCHEMA);
}

bool Url::hasHost()
{
  return fieldset_.test(UF_HOST);
}

bool Url::hasPort()
{
  return fieldset_.test(UF_PORT);
}

bool Url::hasPath()
{
  return fieldset_.test(UF_PATH);
}

bool Url::hasQuery()
{
  return fieldset_.test(UF_QUERY);
}
bool Url::hasFragment()
{
  return fieldset_.test(UF_FRAGMENT);
}
bool Url::hasUserinfo()
{
  return fieldset_.test(UF_USERINFO);
}

Url::~Url(){
}


