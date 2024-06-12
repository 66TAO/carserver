#pragma once
#include "../Scheduler/UsageEnvironment.h"
#include "../Scheduler/Event.h"
#include "Buffer.h"

class TcpConnection
{
public:
    typedef void (*DisConnectCallback)(void*, int);

    TcpConnection(UsageEnvironment* env, int clientFd);
    TcpConnection() {};
    virtual ~TcpConnection();

    void setDisConnectCallback(DisConnectCallback cb, void* arg);

protected:
    void enableReadHandling();
    void enableWriteHandling();
    void enableErrorHandling();
    void disableReadeHandling();
    void disableWriteHandling();
    void disableErrorHandling();

    void handleRead();
    virtual void handleReadBytes(int client_socket);
    virtual void handleWrite();
    virtual void handleError();

    void handleDisConnect();

private:
    static void readCallback(void* arg);
    static void writeCallback(void* arg);
    static void errorCallback(void* arg);

protected:
    UsageEnvironment* mEnv;
    int mClientFd;
    IOEvent* mClientIOEvent;
    DisConnectCallback mDisConnectCallback;//��TrServerʵ�������������ʵ��ʱ�����õĻص�����
    void* mArg;
    Buffer mInputBuffer;
    char mBuffer[2048];
};
