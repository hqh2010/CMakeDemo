#include "llcmdHttpUtils.h"

// libcurl4-gnutls-dev
#include <curl/curl.h>
#include <gio/gio.h>
#include <glib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>

//#include<pthread.h>
using namespace std;
// 锁变量
// pthread_mutex_t mutex;

/*
    Http请求结束的回调函数
    @ 会被调用多次，有下载到数据(http文件)就回调，直到下载完
 */
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

// https://zhuanlan.zhihu.com/p/376295430
int llcmdLock()
{
    const char *lock_file = "/tmp/llcmd.lock";
    int fd = -1;
    int ret = -1;
    fd = open(lock_file, O_CREAT | O_TRUNC | O_RDWR, 0777);

    if (fd < 0)
    {
        fprintf(stdout, "errmsg: [%d] %s\n", errno, strerror(errno));
        return -1;
    }

    ret = flock(fd, LOCK_NB | LOCK_EX);
    if (ret < 0)
    {
        fprintf(stdout, "errmsg: [%d] %s\n", errno, strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}

int llcmdReleaseLock(int fd)
{
    if (fd == -1)
    {
        return fd;
    }
    return fd;
    flock(fd, LOCK_UN);
    close(fd);
    unlink("/tmp/llcmd.lock");
    return 0;
}

int folder_mkdirs(char *folder_path)
{
    if (!access(folder_path, F_OK))
    { /* 判断目标文件夹是否存在 */
        return 1;
    }

    char cmd[512];
    pid_t result;
    fprintf(stdout, "folder_mkdirs len:%d\n", strlen(folder_path));
    // cmd = (char *)malloc(256);
    memset(cmd, '\0', 256);
    snprintf(cmd, 512, "mkdir -p %s", folder_path);
    result = system(cmd);

    // free(cmd);

    if (-1 == result || !WIFEXITED(result) || 0 != WEXITSTATUS(result))
    {
        return 2;
    }

    return 1;
}

int llcmdMkdir()
{
    char path[512];
    getcwd(path, 512);  //取得当前工作目录
    fprintf(stdout, "current dirctory is:%s\n", path);
    // if (mkdir("/home/ut_huqinghong/Cppprog/5.llcmd/222/333/444/555", 0755) == -1)
    // {
    //     fprintf(stdout, "errmsg: [%d] %s\n", errno, strerror(errno));
    //     return -1;
    // }
    folder_mkdirs("/home/ut_huqinghong/Cppprog/5.llcmd/222/333/444/555");

    if (g_mkdir_with_parents("/home/ut_huqinghong/Cppprog/5.llcmd/111/222/333/444", 0755) != 0)
    {
        fprintf(stdout, "g_mkdir_with_parents error");
    }
    return 0;
}

// dltotal是需要下载的总字节数，dlnow是已经下载的字节数。ultotal是将要上传的字节数，ulnow是已经上传的字节数
int ll_progress_callback(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow)
{
    static int i = 0;
    i++;
    if ((i % 1000 == 0 && dltotal > 0) || (abs(dltotal - dlnow) < 0.0001 && dlnow > 0))
    {
        int nPersent = (int)(100.0 * dlnow / dltotal);
        cout << "[" << nPersent << "%]Loading..." << endl;
        // cout << "total:" << dltotal << ", dlnow:" << dlnow << endl;
    }

    return 0;
}

long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

// https://cdn.zoom.us/prod/5.7.29123.0808/zoom_x86_64.tar.xz
// http://10.20.52.184/hqhdebstore/pool/main/l/llcmd/llcmd_1.0.0-1_amd64.deb llcmd_1.0.0-1_amd64.deb
// http://10.20.52.184/hqhdebstore/pool/1.jpg
int llcmd_load_http_uri(char *url, char *filename)
{
    //加锁
    // pthread_mutex_lock(&mutex);
    int fd = llcmdLock();
    if (fd == -1)
    {
        fprintf(stdout, "llcmd_load_http_uri is downloading, please wait a moment and retry\n");
        return -1;
    }
    long start = getCurrentTime();
    CURL *curl_handle;
    // static const char *pagefilename = "page.out";

    FILE *pagefile;

    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* set URL to get here */
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

    curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 3600);  // 超时(单位S)

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_LIMIT, 1);  //如果15秒内平均速度低于每秒1字节就停止

    curl_easy_setopt(curl_handle, CURLOPT_LOW_SPEED_TIME, 15);

    // curl_easy_setopt(curl_handle, CURLOPT_HEADER, 1);             // 下载数据包括HTTP头部
    // 此时若保存文件侧文件数据打不开

    /* Switch on full protocol/debug output while testing */
    // curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    /* disable progress meter, set to 0L to enable it */
    curl_easy_setopt(curl_handle, CURLOPT_PROGRESSFUNCTION, ll_progress_callback);
    curl_easy_setopt(curl_handle, CURLOPT_PROGRESSDATA, NULL);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    long resCode = 0;
    /* open the file */
    pagefile = fopen(filename, "wb");
    if (pagefile)
    {
        /* write the page body to this file handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

        /* get it! */
        CURLcode code = curl_easy_perform(curl_handle);

        if (code != CURLE_OK)
        {
            cout << "curl_easy_perform err code:" << curl_easy_strerror(code) << endl;
            curl_easy_cleanup(curl_handle);
            curl_global_cleanup();
            fclose(pagefile);
            //解锁
            // pthread_mutex_unlock(&mutex);
            llcmdReleaseLock(fd);
            return -1;
        }

        code = curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &resCode);
        if (code != CURLE_OK)
        {
            cout << "1.curl_easy_getinfo err:" << curl_easy_strerror(code) << endl;
        }
        //获取下载文件的大小 字节
        double fileSize = 0;
        code = curl_easy_getinfo(curl_handle, CURLINFO_SIZE_DOWNLOAD, &fileSize);
        if (code != CURLE_OK)
        {
            cout << "2.curl_easy_getinfo err:" << curl_easy_strerror(code) << endl;
        }
        //下载内容大小
        double contentSize = 0;
        code = curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &contentSize);
        if (code != CURLE_OK)
        {
            cout << "3.curl_easy_getinfo err:" << curl_easy_strerror(code) << endl;
        }
        //下载文件类型 text/html application/x-tar application/x-debian-package image/jpeg
        // Http Header里的Content-Type一般有这三种：
        // application/x-www-form-urlencoded：数据被编码为名称/值对。这是标准的编码格式。
        // multipart/form-data： 数据被编码为一条消息，页上的每个控件对应消息中的一个部分。
        // text/plain
        char *contentType = NULL;
        code = curl_easy_getinfo(curl_handle, CURLINFO_CONTENT_TYPE, &contentType);
        if (code != CURLE_OK)
        {
            cout << "4.curl_easy_getinfo err:" << curl_easy_strerror(code) << endl;
        }
        //获取下载总耗时包括域名解析、TCP连接
        double spendTime = 0;
        code = curl_easy_getinfo(curl_handle, CURLINFO_TOTAL_TIME, &spendTime);
        if (code != CURLE_OK)
        {
            cout << "5.curl_easy_getinfo err:" << curl_easy_strerror(code) << endl;
        }

        //下载速度 单位字节
        double downloadSpeed = 0;
        code = curl_easy_getinfo(curl_handle, CURLINFO_SPEED_DOWNLOAD, &downloadSpeed);
        if (code != CURLE_OK)
        {
            cout << "6.curl_easy_getinfo err:" << curl_easy_strerror(code) << endl;
        }
        cout << "Http Respond Code:" << resCode << endl;
        cout << "Http file size:" << (long)fileSize << endl;
        cout << "Http Content size:" << (long)contentSize << endl;
        cout << "Http Content type:" << contentType << endl;
        cout << "Http total time:" << spendTime << " s" << endl;
        cout << "Http download speed:" << downloadSpeed / 1024 << " kb/s" << endl;

        /* close the header file */
        fclose(pagefile);
    }
    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    long end = getCurrentTime();
    cout << "the program spend time is: " << end - start << " ms" << endl;
    //解锁
    // pthread_mutex_unlock(&mutex);
    llcmdReleaseLock(fd);
    if (resCode != 200)
    {
        //删除空文件 to do
        return false;
    }
    return 0;
}
