#include "TrServer.h"
#include "TrConnection.h"
#include "../Base/Log.h"
#include "../Scheduler/SocketsOps.h"
#include"../Scheduler/ThreadPool.h"
#include <thread>



TrServer* TrServer::createNew(UsageEnvironment* env, Ipv4Address& addr, TrDatabase* trdb) {

    return new TrServer(env, addr, trdb);
}
TrServer::TrServer(UsageEnvironment* env, Ipv4Address& addr, TrDatabase* trdb) :
    mEnv(env),
    mAddr(addr),
    Trdb(trdb),
    mListen(false)
{
    mFd = sockets::createTcpSock();															//��������������
    sockets::setReuseAddr(mFd, 1);															//�����׽���ѡ����ö˿ڣ�������ʱ���ܶ˿ڲ�δ�ͷţ����Ϻ���Լ�ʱ���ö˿ڣ�
    if (!sockets::bind(mFd, addr.getIp(), mAddr.getPort())) {
        return;
    }

    LOGI("rtsp://%s:%d fd=%d", addr.getIp().data(), addr.getPort(), mFd);					//����˵�ip���˿ڵ�ǰ�׽���

    mAcceptIOEvent = IOEvent::createNew(mFd, this);
    mAcceptIOEvent->setReadCallback(readCallback);											//����Ҫ���ݴ���//���ûص���socket�ɶ� ����ָ��˿ڽ��������¼�
    mAcceptIOEvent->enableReadHandling();													//����Ϊ�ɶ�

    mCloseTriggerEvent = TriggerEvent::createNew(this);
    mCloseTriggerEvent->setTriggerCallback(cbCloseConnect);									//���ûص��Ĺر����� ����ָ��

    ReadTask = Task::createNew();
    ReadTask->setTaskCallback(TaskCallback, this);

}

TrServer::~TrServer()
{
    if (mListen)
        mEnv->scheduler()->removeIOEvent(mAcceptIOEvent);

    delete mAcceptIOEvent;
    delete mCloseTriggerEvent;

    if (ReadTask) {
        delete ReadTask;
    }

    sockets::close(mFd);
}

void TrServer::thread_process_send_message() {
	try {
        boost::asio::io_context io_context;

        // ����һ��TCP������������8080�˿�
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

        std::cout << "Server is running on port 1234..." << std::endl;

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            // ��ȡ�ͻ�������
            boost::asio::streambuf request;
            boost::asio::read_until(socket, request, "\r\n");

            std::istream request_stream(&request);
            std::string request_method;
            std::string request_uri;
            std::string http_version;
            request_stream >> request_method >> request_uri >> http_version;

            std::string response = Trdb->handle_request(request_uri);
			//std::cout << response << std::endl;
            // ������Ӧ
            boost::asio::write(socket, boost::asio::buffer(response));
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    //try {
    //    boost::asio::io_context io_context;

    //    // ����һ��TCP������������1234�˿�
    //    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

    //    std::cout << "Server is running on port 1234..." << std::endl;

    //    for (;;) {
    //        tcp::socket socket(io_context);
    //        acceptor.accept(socket);

    //        try {
    //            // ��ȡ�ͻ�������
    //            boost::asio::streambuf request;
    //            boost::asio::read_until(socket, request, "\r\n\r\n");

    //            std::istream request_stream(&request);
    //            std::string request_method;
    //            std::string request_uri;
    //            std::string http_version;
    //            request_stream >> request_method >> request_uri >> http_version;

    //            // ��������������Ӧ
    //            std::string response = Trdb->handle_request(request_uri);

    //            // ������Ӧ
    //            boost::asio::write(socket, boost::asio::buffer(response));
    //        }
    //        catch (boost::system::system_error& e) {
    //            if (e.code() == boost::asio::error::eof) {
    //                std::cerr << "Client disconnected: " << e.what() << std::endl;
    //                // �������ѡ����������ر�����
    //            }
    //            else {
    //                throw;  // �����׳��쳣�Բ����������͵Ĵ���
    //            }
    //        }
    //    }
    //}
    //catch (std::exception& e) {
    //    std::cerr << "Exception: " << e.what() << std::endl;
    //}

}

void TrServer::remove_by_value(std::map<std::string, int>& map, const int& value)				// �Ƴ�map���ض���ֵ
{
    for (auto it = map.begin(); it != map.end(); )
    {
        if (it->second == value)
        {
            it = map.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void TrServer::start() {															// ����������
    LOGI("Start thread");
    std::thread t1(&TrServer::thread_process_send_message, this);		// ����һ���߳�ִ����Ϣ��������
    t1.detach();														// �����̣߳�ʹ���ں�̨����
    mListen = true;														// ��Ƿ���������״̬Ϊtrue
    sockets::listen(mFd, 60);											// ��ʼ�����ͻ�����������backlog����Ϊ60��������Ӹ�����
    mEnv->scheduler()->addIOEvent(mAcceptIOEvent);						// ��Accept�¼���ӵ��¼���������
    Trdb->init_db();													// ��ʼ�����ݿ�
}

void TrServer::readCallback(void* arg) {								// ���ж������Ļص�����
    TrServer* trServer = (TrServer*)arg;
    trServer->handleRead();												//����Ҫ�����ݴ���

}

void TrServer::handleRead() {											//����mAcceptIOEvent// ������¼�
    int clientFd = sockets::accept(mFd);								// ���ܿͻ������ӣ���ȡ�ͻ����׽����ļ�������
    if (clientFd < 0)
    {
        LOGE("handleRead error,clientFd=%d", clientFd);
        return;
    }
    TrConnection* conn = TrConnection::createNew(this, Trdb, clientFd);		// �����µ����Ӷ���    ����Ҫ�����ݴ���
    conn->setDisConnectCallback(TrServer::cbDisConnect, this);				// �������ӶϿ��ص�����
    mConnMap.insert(std::make_pair(clientFd, conn));						// �����Ӷ����������ӳ�����

}
void TrServer::cbDisConnect(void* arg, int clientFd) {					// ���ӶϿ��Ļص�����
    TrServer* server = (TrServer*)arg;

    server->handleDisConnect(clientFd);
}

void TrServer::handleDisConnect(int clientFd) {								// �������ӶϿ��¼�

    remove_by_value(device_match, clientFd);							// ���豸ӳ������Ƴ��Ͽ����ӵ��豸

    mDisConnList.push_back(clientFd);									// ���Ͽ����ӵĿͻ����׽����ļ���������ӵ��Ͽ������б���

    mEnv->scheduler()->addTriggerEvent(mCloseTriggerEvent);				// ��ӹر����Ӵ����¼����¼���������

}

void TrServer::cbCloseConnect(void* arg) {							// �ر����ӵĻص�����
    TrServer* server = (TrServer*)arg;
    server->handleCloseConnect();
}

void TrServer::handleCloseConnect() {																// ����ر������¼�


    for (std::vector<int>::iterator it = mDisConnList.begin(); it != mDisConnList.end(); ++it) {

        int clientFd = *it;									//itָ��ļ�ֵ���еļ���ֵ��clientFd
        std::map<int, TrConnection*>::iterator _it = mConnMap.find(clientFd);
        assert(_it != mConnMap.end());						//���ж��Լ����ָ���Ƿ���Ч�������Чֱ����ֹ����
        delete _it->second;																			// ɾ�����Ӷ���
        mConnMap.erase(clientFd);																	// ������ӳ������Ƴ�����
        remove_by_value(device_match, clientFd);													// ���豸ӳ������Ƴ��Ͽ����ӵ��豸
    }

    mDisConnList.clear();																		// ��նϿ������б�

}

void TrServer::TaskCallback(void* arg)
{
    //TrServer* trServer = (TrServer*)arg;
    //trServer->handleRead();												//����Ҫ�����ݴ���
}