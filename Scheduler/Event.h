#pragma once
typedef void (*EventCallback)(void*);//定义一个函数指针（指向返回值为void并且有一个void*参数的函数）（类中定义加typedef）
//typedef void(*T)(void*);    T t;    void func(void*)；   t = func;
class TriggerEvent {
public:
    static TriggerEvent* createNew(void* arg);
    static TriggerEvent* createNew();

    TriggerEvent(void* arg);
    ~TriggerEvent();

    void setArg(void* arg) { mArg = arg; }
    void setTriggerCallback(EventCallback cb) { mTriggerCallback = cb; }
    void handleEvent();

private:
    void* mArg;
    EventCallback mTriggerCallback;
};

class TimerEvent {
public:
    static TimerEvent* createNew(void* arg);
    static TimerEvent* createNew();

    TimerEvent(void* arg);
    ~TimerEvent();

    void setArg(void* arg) { mArg = arg; }
    void setTimeoutCallback(EventCallback cb) { mTimeoutCallback = cb; }
    bool handleEvent();

    void stop();

private:
    void* mArg;
    EventCallback mTimeoutCallback;
    bool mIsStop;
};

class IOEvent {
public:
    enum IOEventType
    {
        EVENT_NONE = 0,
        EVENT_READ = 1,
        EVENT_WRITE = 2,
        EVENT_ERROR = 4,
    };

    static IOEvent* createNew(int fd, void* arg);
    static IOEvent* createNew(int fd);

    IOEvent(int fd, void* arg);
    ~IOEvent();

    int getFd() const { return mFd; }
    int getEvent() const { return mEvent; }
    void setREvent(int event) { mREvent = event; }
    void setArg(void* arg) { mArg = arg; }

    void setReadCallback(EventCallback cb) { mReadCallback = cb; };
    void setWriteCallback(EventCallback cb) { mWriteCallback = cb; };
    void setErrorCallback(EventCallback cb) { mErrorCallback = cb; };

    void enableReadHandling() { mEvent |= EVENT_READ; }
    void enableWriteHandling() { mEvent |= EVENT_WRITE; }
    void enableErrorHandling() { mEvent |= EVENT_ERROR; }
    void disableReadeHandling() { mEvent &= ~EVENT_READ; }
    void disableWriteHandling() { mEvent &= ~EVENT_WRITE; }
    void disableErrorHandling() { mEvent &= ~EVENT_ERROR; }

    bool isNoneHandling() const { return mEvent == EVENT_NONE; }
    bool isReadHandling() const { return (mEvent & EVENT_READ) != 0; }
    bool isWriteHandling() const { return (mEvent & EVENT_WRITE) != 0; }
    bool isErrorHandling() const { return (mEvent & EVENT_ERROR) != 0; };

    void handleEvent();

private:
    int mFd;
    void* mArg;
    int mEvent;
    int mREvent;
    EventCallback mReadCallback;
    EventCallback mWriteCallback;
    EventCallback mErrorCallback;
};

