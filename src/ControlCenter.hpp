

#ifndef __CONTROL_CENTER_HPP__
#define __CONTROL_CENTER_HPP__

#include <string>
#include <vector>
#include <stdint.h>
#include "Downloader.hpp"
#include "Worker.hpp"


class ControlCenter
{
public:
    enum Config{
        ChunkSize = 256 << 10, /* 256K */
    };

public:
    ControlCenter()
    {
        pthread_mutex_init(&mMutex, NULL);
        mFD = -1;
        mExitCode = 0;
        mFileSize = 0;
        mServerSupportRange = false;
    }
    ~ControlCenter()
    {
        pthread_mutex_destroy(&mMutex);
    }

public:
    int Init(const std::string& url, const std::string& protoType);

    int Finit();

    int StartWork();

    int GetFileSize();

    inline int GetExitCode()
    {
        int exitCode = 0;
        pthread_mutex_lock(&mMutex);
        exitCode = mExitCode;
        pthread_mutex_unlock(&mMutex);

        return exitCode;
    }

    inline void SetExitCode(int exitCode)
    {
        pthread_mutex_lock(&mMutex);
        mExitCode = exitCode;
        pthread_mutex_unlock(&mMutex);
    }

    inline bool IsSystemRunning()
    {
        bool ret = true;
        pthread_mutex_lock(&mMutex);
        if (0 != mExitCode) {
            ret = false;
        }
        pthread_mutex_unlock(&mMutex);

        return ret;
    }

private:
    int CreateWorkers();
    void WaitStop();
    uint32_t GetWorkerCount();
    Downloader *NewDownloader();

private:
    std::string mURL;
    std::string mProtoType;
    bool mServerSupportRange;
    int mFD;
    size_t mFileSize;
    std::string mFileName;

    int mExitCode;
    pthread_mutex_t mMutex;
    std::vector<Worker*> mWorkers;
    std::vector<Downloader*> mDownLoaders;
};

#endif


