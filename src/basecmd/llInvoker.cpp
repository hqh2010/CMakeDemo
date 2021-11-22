#include "llInvoker.h"

#include "../speccmd/HelpCmd.h"
#include "../speccmd/InstallCmd.h"
#include "../speccmd/UpdateCmd.h"
#include "../utils/LogUtils.h"

using namespace std;

llInvoker::llInvoker()
{
    mMap.insert(map<string, llBaseCmd *>::value_type("install", new InstallCmd()));
    mMap.insert(map<string, llBaseCmd *>::value_type("update", new UpdateCmd()));
}

llInvoker::~llInvoker()
{
    // cout<<"llInvoker release resource:"<<mMap.size()<<endl;
    LogUtils::log_info("llInvoker release resource:" + std::to_string(mMap.size()));
    for (auto it = mMap.begin(); it != mMap.end(); ++it)
    {
        if (it->second)
        {
            delete it->second;
        }
    }
    mMap.clear();
}

int llInvoker::runCmd(int argc, char **argv)
{
    if (argc < 2)
    {
        // cout << "runCmd argc:" << argc << endl;
        LogUtils::log_info("runCmd argc:" + std::to_string(argc));
        return 0;
    }
    // cout << "runCmd:llcmd " << argv[1] << endl;
    LogUtils::log_info("runCmd:llcmd " + (string)argv[1]);
    string key = argv[1];
    auto it = mMap.find(key);
    if (it != mMap.end())
    {
        it->second->execCmd(argc, argv);
    }
    else
    {
         HelpCmd* p = new HelpCmd();
         p->execCmd(argc, argv);
         delete p;
    }
    return 0;
}