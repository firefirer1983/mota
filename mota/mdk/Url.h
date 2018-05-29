#ifndef MOTA_MDK_URL_H
#define MOTA_MDK_URL_H
#include <string>
#include <bitset>
#include <vector>
#include <muduo/base/copyable.h>

namespace mota
{
namespace mdk
{

class Url: muduo::copyable
{
public:
  Url(const char *url="");
  ~Url();
//  bool hasHost();
  const char *schema();
  const char *host();
  unsigned short port();
  const char *path();
  const char *query();
  const char *fragment();
  const char *userinfo();

  bool hasSchema();
  bool hasHost();
  bool hasPort();
  bool hasPath();
  bool hasQuery();
  bool hasFragment();
  bool hasUserinfo();
private:
  std::vector<std::string> parsed_;
  std::bitset<16> fieldset_;
  std::string url_;
};


}
}

#endif
