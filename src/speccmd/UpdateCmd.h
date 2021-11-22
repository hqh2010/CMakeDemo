#include "../basecmd/llBaseCmd.h"

class UpdateCmd:public llBaseCmd
{
  public:
    UpdateCmd();
    ~UpdateCmd();
    virtual int extractCmd(int argc, char** argv);
    virtual int verifyCmd();
    virtual int execCmd(int argc, char** argv);
};
