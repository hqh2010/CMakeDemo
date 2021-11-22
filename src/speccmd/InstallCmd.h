#include "../basecmd/llBaseCmd.h"

class InstallCmd:public llBaseCmd
{
  public:
    InstallCmd(/* args */);
    ~InstallCmd();
    virtual int extractCmd(int argc, char** argv);
    virtual int verifyCmd();
    virtual int execCmd(int argc, char** argv);
};