#ifndef __LLINVOKER_H__
#define __LLINVOKER_H__
#include <iostream>
#include <map>
#include <string>
using namespace std;
#include "llBaseCmd.h"

class llInvoker
{
  public:
    llInvoker(/* args */);
    ~llInvoker();
    int runCmd(int argc, char** argv);
  private:
    map<string, llBaseCmd*> mMap;
};
#endif