#include "llBaseCmd.h"

#include <iostream>

#include "../utils/LogUtils.h"
using namespace std;

llBaseCmd::llBaseCmd(/* args */)
{
    // cout<<"llBaseCmd constructor called"<<endl;
    LogUtils::log_info("llBaseCmd constructor called");
}

llBaseCmd::~llBaseCmd()
{
    // cout<<"~llBaseCmd destructor called"<<endl;
    LogUtils::log_info("~llBaseCmd destructor called");
}