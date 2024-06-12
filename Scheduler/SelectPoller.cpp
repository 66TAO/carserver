#include "SelectPoller.h"
#include "../Base/Log.h"

SelectPoller::SelectPoller()
{
    FD_ZERO(&mReadSet);                                             // ��ն��ļ�����������
    FD_ZERO(&mWriteSet);                                            // ���д�ļ�����������
    FD_ZERO(&mExceptionSet);                                        // ����쳣�ļ�����������
}

SelectPoller::~SelectPoller()
{

}

SelectPoller* SelectPoller::createNew()                             // SelectPoller��ľ�̬���������ڴ���һ���µ�SelectPoller����
{
    return new SelectPoller();                                      // ����һ���µ�SelectPoller����
    //    return New<SelectPoller>::allocate();
}

bool SelectPoller::addIOEvent(IOEvent* event)                       // ���һ��IO�¼�����ѯ����
{
    return updateIOEvent(event);                                    // ���ø���IO�¼��ķ���
}

bool SelectPoller::updateIOEvent(IOEvent* event)                    // ������ѯ���е�һ��IO�¼�
{
    int fd = event->getFd();                                        // ��ȡ�¼����ļ�������
    if (fd < 0)
    {
        LOGE("fd=%d", fd);                                          // ���������־

        return false;
    }

    FD_CLR(fd, &mReadSet);                                          // ����ļ���������Ӧ���¼�����
    FD_CLR(fd, &mWriteSet);
    FD_CLR(fd, &mExceptionSet);

    IOEventMap::iterator it = mEventMap.find(fd);                   // �����¼��Ƿ��Ѿ���ӵ��¼�ӳ����
    if (it != mEventMap.end())                                      //��ǰ�Ѿ�������޸�  ����     // �����¼�����������Ӧ���¼�����
    {
        if (event->isReadHandling())
            FD_SET(fd, &mReadSet);
        if (event->isWriteHandling())
            FD_SET(fd, &mWriteSet);
        if (event->isErrorHandling())
            FD_SET(fd, &mExceptionSet);
    }
    else                                                    //��ǰδ��������IO�¼�  ������     // �����¼�����������Ӧ���¼�����
    {
        if (event->isReadHandling())
            FD_SET(fd, &mReadSet);
        if (event->isWriteHandling())
            FD_SET(fd, &mWriteSet);
        if (event->isErrorHandling())
            FD_SET(fd, &mExceptionSet);

        mEventMap.insert(std::make_pair(fd, event));                    // ���¼���ӵ��¼�ӳ����
    }

    if (mEventMap.empty())                                              // ��������ļ���������Ŀ
        mMaxNumSockets = 0;
    else
        mMaxNumSockets = mEventMap.rbegin()->first + 1;                 //��������ļ�������+1��map���Զ��������һλ

    return true;
}

bool SelectPoller::removeIOEvent(IOEvent* event)                            // ����ѯ�����Ƴ�һ��IO�¼�
{
    int fd = event->getFd();
    if (fd < 0)
        return false;

    FD_CLR(fd, &mReadSet);                                                  // ����ļ���������Ӧ���¼�����
    FD_CLR(fd, &mWriteSet);
    FD_CLR(fd, &mExceptionSet);

    IOEventMap::iterator it = mEventMap.find(fd);                           // ���¼�ӳ�����Ƴ��¼�
    if (it != mEventMap.end())
        mEventMap.erase(it);

    if (mEventMap.empty())                                                  // ��������ļ���������Ŀ
        mMaxNumSockets = 0;
    else
        mMaxNumSockets = mEventMap.rbegin()->first + 1;

    return true;
}

void SelectPoller::handleEvent() {                                          // �����¼��ķ���

    fd_set readSet = mReadSet;
    fd_set writeSet = mWriteSet;
    fd_set exceptionSet = mExceptionSet;
    struct timeval timeout;
    int ret;
    int rEvent;

    timeout.tv_sec = 1000;// ��
    timeout.tv_usec = 0;//΢��
    //LOGI("mEventMap.size() = %d��select io start", mEventMap.size());

    ret = select(mMaxNumSockets, &readSet, &writeSet, &exceptionSet, &timeout);                 // ����select���������¼���ѯ


    if (ret < 0) {                                                          // ���select��������ֵС��0�����ʾ��������ֱ�ӷ���
        return;
    }
    else {
        //LOGI("��⵽��Ծ�������� ret=%d", ret);
    }
    //LOGI("select io end");

    for (IOEventMap::iterator it = mEventMap.begin(); it != mEventMap.end(); ++it)          // �����¼�ӳ�䣬�����Щ�ļ����������¼�����
    {
        rEvent = 0;
        if (FD_ISSET(it->first, &readSet))                              // �����¼��Ƿ���
            rEvent |= IOEvent::EVENT_READ;

        if (FD_ISSET(it->first, &writeSet))
            rEvent |= IOEvent::EVENT_WRITE;

        if (FD_ISSET(it->first, &exceptionSet))
            rEvent |= IOEvent::EVENT_ERROR;

        if (rEvent != 0)                                            // ������¼��������������¼��ľ����¼��������¼�����������¼��б�
        {
            it->second->setREvent(rEvent);
            mIOEvents.push_back(it->second);
        }
    }

    for (auto& ioEvent : mIOEvents) {                           // ����������¼��б��е������¼�
        ioEvent->handleEvent();
    }

    mIOEvents.clear();                                      // ��մ������¼��б�

}



