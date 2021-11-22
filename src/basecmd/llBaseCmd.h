#ifndef __LLBASECMD_H__
#define __LLBASECMD_H__
class llBaseCmd
{
  private:
  public:
    llBaseCmd(/* args */);
    virtual ~llBaseCmd();
    virtual int extractCmd(int argc, char** argv) { return 0; }
    virtual int verifyCmd() { return 0; }
    virtual int execCmd(int argc, char** argv) { return 0; }
};
#endif