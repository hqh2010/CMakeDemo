#include "UpdateCmd.h"

#include <iostream>

#include "../utils/LogUtils.h"
using namespace std;

UpdateCmd::UpdateCmd() {}

UpdateCmd::~UpdateCmd() {}

int UpdateCmd::extractCmd(int argc, char** argv)
{
    // cout << "UpdateCmd extractCmd" << endl;
    LogUtils::log_info("UpdateCmd extractCmd");
    return 0;
}

int UpdateCmd::verifyCmd()
{
    // cout << "UpdateCmd verifyCmd" << endl;
    LogUtils::log_info("UpdateCmd verifyCmd");
    return 0;
}

int UpdateCmd::execCmd(int argc, char** argv)
{
    // cout << "UpdateCmd execCmd" << endl;
    LogUtils::log_info("UpdateCmd execCmd");
    return 0;
}