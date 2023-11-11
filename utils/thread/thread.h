#ifndef THREAD_H
#define THREAD_H

#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>

enum Priority {
    PRIORITY_LOWEST = 0,
    PRIORITY_LOW,
    PRIORITY_NORMAL,
    PRIORITY_HIGH,
    PRIORITY_HIGHEST
};

class Thread {
public:
    explicit Thread(std::string threadName);

    virtual ~Thread() = default;

    virtual void start();

    virtual void stop();

    virtual void pause();

    virtual void resume();

    virtual void join();

protected:
    virtual bool threadLoop() = 0;
    virtual bool prepare();

    virtual void setThreadName(const char *name);
    virtual bool setThreadAffinity(int id);
    virtual bool setThreadPriority(Priority priority = PRIORITY_NORMAL, std::thread::native_handle_type threadId = 0);

private:
    void interrupt(bool);

    void run();

protected:
    std::thread thread_;
    std::atomic<bool> interrupted_;
    std::atomic<bool> started_;
    std::atomic<bool> paused_;
    std::atomic<bool> joined_;
    std::condition_variable cond_;
    std::mutex mutex_;
    std::string threadName_;
};

#endif //THREAD_H
