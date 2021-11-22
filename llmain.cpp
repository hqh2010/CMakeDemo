#include <gio/gio.h>
#include <glib.h>

#include <iostream>

#include "src/basecmd/llInvoker.h"
#include "src/utils/llcmdHttpUtils.h"
using namespace std;
int main(int argc, char **argv)
{
    // llInvoker *p = new llInvoker();
    // 调试参数也可以在 vs code中指定
    // int argcd = 3;
    // char *argvd[] = {"path", "--help", "123"};
    // cout << "调试参数 argv[1]=" << argvd[1] << ", argv[2]=" << argvd[2] << endl;
    // int ret = p->runCmd(argc, argv);
    // delete p;
    // char *argvd[] = {"path", "https://cdn.zoom.us/prod/5.7.29123.0808/zoom_x86_64.tar.xz", "zoom_x86_64.tar.xz"};

    // const string para_err = __FILE__ + " param err";
    char info[512] = {'\0'};
    snprintf(info, 512, "%s, function:%s param error", __FILE__, __func__);
    string infoStr = info;
    cout << infoStr << endl;
#ifdef DEBUG
    //g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, message_handler, NULL);
    printf("This is debug mode\n");
#endif  // DEBUG
    g_debug("123");
    if (argc >= 3)
    {
        llcmd_load_http_uri(argv[1], argv[2]);
    }
    llcmdMkdir();
    return 0;
}
