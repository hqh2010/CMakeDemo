#include "InstallCmd.h"

#include <iostream>

#include "../utils/LogUtils.h"
using namespace std;
InstallCmd::InstallCmd() {}

InstallCmd::~InstallCmd() {}

int InstallCmd::extractCmd(int argc, char** argv)
{
    // cout << "InstallCmd extractCmd" << endl;
    LogUtils::log_info("InstallCmd extractCmd");
    return 0;
}

int InstallCmd::verifyCmd()
{
    // cout << "InstallCmd verifyCmd" << endl;
    LogUtils::log_info("InstallCmd verifyCmd");
    return 0;
}

int InstallCmd::execCmd(int argc, char** argv)
{
    // cout << "InstallCmd execCmd" << endl;
    LogUtils::log_info("InstallCmd execCmd");
    return 0;
}