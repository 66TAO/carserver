#include "TcpConnection.h"
#include "../Scheduler/SocketsOps.h"
#include "../Base/Log.h"


TcpConnection::TcpConnection(UsageEnvironment* env, int clientFd) :
    mEnv(env),
    mClientFd(clientFd)
{
    mClientIOEvent = IOEvent::createNew(clientFd, this);                    // ����һ���µ� IOEvent ����������ͻ����ļ��������� I/O �¼�
    mClientIOEvent->setReadCallback(readCallback);                          //����Ҫ����database// ���ö���д�ʹ���Ļص�����
    mClientIOEvent->setWriteCallback(writeCallback);
    mClientIOEvent->setErrorCallback(errorCallback);
    mClientIOEvent->enableReadHandling(); //Ĭ��ֻ������

    //    mClientIOEvent->enableWriteHandling();
    //    mClientIOEvent->enableErrorHandling();

    mEnv->scheduler()->addIOEvent(mClientIOEvent);//����ͻ���IO�¼�           // ���ͻ��� IOEvent ��ӵ��������У��Ա�����¼�����
}

TcpConnection::~TcpConnection()
{
    mEnv->scheduler()->removeIOEvent(mClientIOEvent);                   // �ӵ��������Ƴ��ͻ��� IOEvent
    delete mClientIOEvent;                                              // ɾ���ͻ��� IOEvent ����
    //    Delete::release(mClientIOEvent);

    sockets::close(mClientFd);                                          // �رտͻ����ļ�������
}

void TcpConnection::setDisConnectCallback(DisConnectCallback cb, void* arg)         // ���öϿ����ӻص������������
{
    mDisConnectCallback = cb;
    mArg = arg;
}

void TcpConnection::enableReadHandling()                                // ���ö�ȡ����
{
    if (mClientIOEvent->isReadHandling())                               // ����Ѿ������˶�ȡ������ֱ�ӷ���
        return;

    mClientIOEvent->enableReadHandling();                               // ������ȡ����
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);                   // ���¿ͻ��� IOEvent ���¼�״̬����������
}

void TcpConnection::enableWriteHandling()
{
    if (mClientIOEvent->isWriteHandling())
        return;

    mClientIOEvent->enableWriteHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::enableErrorHandling()
{
    if (mClientIOEvent->isErrorHandling())
        return;

    mClientIOEvent->enableErrorHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableReadeHandling()                          // ���ö�ȡ����
{
    if (!mClientIOEvent->isReadHandling())                          // ���δ������ȡ������ֱ�ӷ���
        return;

    mClientIOEvent->disableReadeHandling();                         // ���ö�ȡ����
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);               // ���¿ͻ��� IOEvent ���¼�״̬����������
}

void TcpConnection::disableWriteHandling()
{
    if (!mClientIOEvent->isWriteHandling())
        return;

    mClientIOEvent->disableWriteHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::disableErrorHandling()
{
    if (!mClientIOEvent->isErrorHandling())
        return;

    mClientIOEvent->disableErrorHandling();
    mEnv->scheduler()->updateIOEvent(mClientIOEvent);
}

void TcpConnection::handleRead() {                                  // �����ȡ�¼�

    //LOGI("");
    //vector<char> ret = mInputBuffer.read(mClientFd);                         // �ӿͻ��˶�ȡ���ݵ����뻺����

    //if (ret.empty())                                                   // �����ȡ������߶�ȡ���ֽ���С�ڵ��� 0������Ϊ�ͻ����Ѿ��Ͽ�����
    //{
    //    LOGE("read error,fd=%d", mClientFd);
    //    handleDisConnect();                                         // ����Ͽ������¼�
    //    return;
    //}

    /* ��ȡ���� */
    //this->disableReadeHandling();

    handleReadBytes(mClientFd);// ����RtspConnecton�����ʵ�ֺ���             // �����ȡ��������         ����Ҫ�������ݺ���
    //std::cout << "File received." << std::endl;
}

void TcpConnection::handleReadBytes(int client_socket) {//TrConnection��д�÷���       ������д
    LOGI("");
}
void TcpConnection::handleDisConnect()
{
    if (mDisConnectCallback) {                                      // ��������˶Ͽ����ӻص�����������øûص�����
        mDisConnectCallback(mArg, mClientFd);
    }
}
void TcpConnection::handleWrite()
{
    LOGI("");

}

void TcpConnection::handleError()
{
    LOGI("");
}

void TcpConnection::readCallback(void* arg)                         //���Ļص�����
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleRead();                                        //����Ҫ���ݴ���
}

void TcpConnection::writeCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleWrite();
}

void TcpConnection::errorCallback(void* arg)
{
    TcpConnection* tcpConnection = (TcpConnection*)arg;
    tcpConnection->handleError();
}

