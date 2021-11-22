#ifndef __LOGUTILS__
#define __LOGUTILS__
#include <string>
using namespace std;

class LogUtils
{
  public:
    static void log_info(string str);
    static void log_err(string str);

  private:
    //static const string TAG;
};

#endif