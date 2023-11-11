#include <cassert>
#include <utility>
#include "ALog.h"
#include "thread.h"

#define  LOG_TAG "Thread"

Thread::Thread(std::string threadName) :
        interrupted_(false),
        started_(false),
        paused_(false),
        joined_(false),
        threadName_(std::move(threadName)) {

}

void Thread::start() {
    assert(!started_);
    interrupt(false);
    thread_ = std::thread(&Thread::run, this);
    started_ = true;
}

void Thread::stop() {
    if (!started_) {
        return;
    }
    if (paused_) {
        resume();
    }
    interrupt(true);
    join();
}

void Thread::pause() {
    paused_ = true;
}

void Thread::resume() {
    paused_ = false;
    cond_.notify_one();
}

void Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    started_ = false;
    if (thread_.joinable()) {
        thread_.join();
    }
}

void Thread::interrupt(bool flag) {
    interrupted_ = flag;
}

static std::string limitString(const char *name, size_t max_size) {
    std::string str = name;
    if (str.size() + 1 > max_size) {
        auto erased = str.size() + 1 - max_size + 3;
        str.replace(5, erased, "...");
    }
    return str;
}

void Thread::setThreadName(const char *name) {
    assert(name);
#if defined(__linux) || defined(__linux__) || defined(__MINGW32__)
    pthread_setname_np(pthread_self(), limitString(name, 16).data());
#elif defined(__MACH__) || defined(__APPLE__)
    pthread_setname_np(limitString(name, 32).data());
#elif defined(_MSC_VER)
    // SetThreadDescription was added in 1607 (aka RS1). Since we can't guarantee the user is running 1607 or later, we need to ask for the function from the kernel.
    using SetThreadDescriptionFunc = HRESULT(WINAPI * )(_In_ HANDLE hThread, _In_ PCWSTR lpThreadDescription);
    static auto setThreadDescription = reinterpret_cast<SetThreadDescriptionFunc>(::GetProcAddress(::GetModuleHandle("Kernel32.dll"), "SetThreadDescription"));
    if (setThreadDescription) {
        // Convert the thread name to Unicode
        wchar_t threadNameW[MAX_PATH];
        size_t numCharsConverted;
        errno_t wcharResult = mbstowcs_s(&numCharsConverted, threadNameW, name, MAX_PATH - 1);
        if (wcharResult == 0) {
            HRESULT hr = setThreadDescription(::GetCurrentThread(), threadNameW);
            if (!SUCCEEDED(hr)) {
                int i = 0;
                i++;
            }
        }
    } else {
        // For understanding the types and values used here, please see:
        // https://docs.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code

        const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)
        struct THREADNAME_INFO {
            DWORD dwType = 0x1000; // Must be 0x1000
            LPCSTR szName;         // Pointer to name (in user address space)
            DWORD dwThreadID;      // Thread ID (-1 for caller thread)
            DWORD dwFlags = 0;     // Reserved for future use; must be zero
        };
#pragma pack(pop)

        THREADNAME_INFO info;
        info.szName = name;
        info.dwThreadID = (DWORD) - 1;

        __try{
                RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<const ULONG_PTR *>(&info));
        } __except(GetExceptionCode() == MS_VC_EXCEPTION ? EXCEPTION_CONTINUE_EXECUTION : EXCEPTION_EXECUTE_HANDLER) {
        }
    }
#else
    thread_name = name ? name : "";
#endif
}

bool Thread::setThreadAffinity(int id) {
#if (defined(__linux) || defined(__linux__)) && !defined(ANDROID)
    cpu_set_t mask;
    CPU_ZERO(&mask);
    if (id >= 0) {
        CPU_SET(id, &mask);
    } else {
        for (auto j = 0u; j < std::thread::hardware_concurrency(); ++j) {
            CPU_SET(j, &mask);
        }
    }
    if (0 != pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask)) {
        LOGE(LOG_TAG, "pthread_setaffinity_np failed!");
        return false;
    }
#endif
    return true;
}

bool Thread::setThreadPriority(Priority priority, std::thread::native_handle_type threadId) {
#if defined(_WIN32)
    static int Priorities[] = { THREAD_PRIORITY_LOWEST, THREAD_PRIORITY_BELOW_NORMAL, THREAD_PRIORITY_NORMAL, THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_HIGHEST };
        if (priority != PRIORITY_NORMAL && SetThreadPriority(GetCurrentThread(), Priorities[priority]) == 0) {
            return false;
        }
        return true;
#else
    static int Min = sched_get_priority_min(SCHED_OTHER);
    if (Min == -1) {
        return false;
    }
    static int Max = sched_get_priority_max(SCHED_OTHER);
    if (Max == -1) {
        return false;
    }
    static int Priorities[] = {Min, Min + (Max - Min) / 4, Min + (Max - Min) / 2, Min + (Max - Min) * 3 / 4, Max};

    if (threadId == 0) {
        threadId = pthread_self();
    }
    struct sched_param params;
    params.sched_priority = Priorities[priority];
    return pthread_setschedparam(threadId, SCHED_OTHER, &params) == 0;
#endif
}

void Thread::run() {
    if (!threadName_.empty()) {
        setThreadName(threadName_.c_str());
    }
    if (!prepare()) {
        return;
    }
    do {
        if (paused_) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [&] {
                return !paused_;
            });
        }

        if (!threadLoop()) {
            started_ = false;
            break;
        }
    } while (!interrupted_);
}

bool Thread::prepare() {
    return true;
}
