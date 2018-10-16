
#include "dmutil.h"
#include "dmsingleton.h"
#include "dmthread.h"
#include "dmconsole.h"
#include "dmtypes.h"
#include "dmlock.h"

#include <mutex>

#define MAX_THREADS 10
#define MAX_LOCK 1000000;

uint64_t gCount = 0;
std::mutex mutexLock;
CLock gLock;

class CMain :
    public IDMConsoleSink,
    public IDMThread,
    public CDMThreadCtrl,
    public TSingleton<CMain>
{
    friend class TSingleton<CMain>;
public:

    virtual void ThrdProc() {

        {
            std::list<std::thread*> threadlist;

            //≤‚ ‘mutex  
            printf("testing mutex...\n");

            clock_t start = clock();
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                threadlist.push_back(new std::thread([] {
                    for (int j = 0; j < 1000000; ++j)
                    {
                        mutexLock.lock();
                        gCount += j;
                        mutexLock.unlock();
                    }
                }));
            }

            for (auto in : threadlist)
            {
                in->join();
            }
            clock_t finish = clock();
            printf("result:%llu\n", gCount);
            printf("cost:%dms\n", finish - start);
        }
        {
            std::list<std::thread*> threadlist;

            //≤‚ ‘mutex  
            printf("testing CLock...\n");

            clock_t start = clock();
            for (int i = 0; i < MAX_THREADS; ++i)
            {
                threadlist.push_back(new std::thread([] {

                    for (int j = 0; j < 1000000; ++j)
                    {
                        gLock.lock();
                        gCount += j;
                        gLock.unlock();
                    }
 
                }));
            }

            for (auto in : threadlist)
            {
                in->join();
            }
            clock_t finish = clock();
            printf("result:%llu\n", gCount);
            printf("cost:%dms\n", finish - start);
        }
    }

    virtual void Terminate() {
        m_bStop = true;
    }

    virtual void OnCloseEvent() {
        Stop();
    }
private:
    CMain()
        : m_bStop(false) {
        HDMConsoleMgr::Instance()->SetHandlerHook(this);
    }

    virtual ~CMain() {

    }

private:
    bool __Run() {
        return false;
    }
private:
    volatile bool   m_bStop;
};

int main(int argc, char* argv[]) {
    CMain::Instance()->Start(CMain::Instance());
    CMain::Instance()->WaitFor();
    return 0;
}
