#include "LogUtils.h"

#include <iostream>

int g_debug_switch = 1;
const string TAG = "LingLong:";
void LogUtils::log_info(string str)
{
    if (g_debug_switch)
    {
        cout << TAG + "info:" + str << endl;
    }
}

void LogUtils::log_err(string str)
{
    if (g_debug_switch)
    {
        cout << TAG + "error:" + str << endl;
    }
}