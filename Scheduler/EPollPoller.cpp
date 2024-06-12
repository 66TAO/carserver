#include "EPollPoller.h"
#include "../Base/Log.h"
#include <fcntl.h>

EPollPoller::EPollPoller()
{
    epollfd = epoll_create(100);

}

EPollPoller::~EPollPoller()
{

}

EPollPoller* EPollPoller::createNew()
{
    return new EPollPoller();                                   // ����һ���µ� EPollPoller ʵ��������ָ��
    //    return New<SelectPoller>::allocate();
}

bool EPollPoller::addIOEvent(IOEvent* event)                    // �� epoll �����һ�� IOEvent    
{
    return updateIOEvent(event);
}

bool EPollPoller::updateIOEvent(IOEvent* event)                 // ��ӻ����һ�� IOEvent
{
    int fd = event->getFd();
    if (fd < 0)
    {
        LOGE("fd=%d", fd);                                      // ��־���ļ���������Ч

        return false;
    }

    Read_events.data.fd = fd;                                   // ���� epoll �¼�����Ϊ���¼�
    Read_events.events = EPOLLIN;

    IOEventMap::iterator it = mEventMap.find(fd);               // ���� IOEvent �Ƿ��Ѿ��������¼�ӳ�����
    if (it != mEventMap.end())                              //��ǰ�Ѿ�������޸�  ����
    {
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &Read_events);
    }
    else                                                    //��ǰδ��������IO�¼�  ������
    {
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &Read_events);
        mEventMap.insert(std::make_pair(fd, event));            // ���¼���ӵ��¼�ӳ�����
    }

    if (mEventMap.empty())                          // ��������ļ�������
        mMaxNumSockets = 0;                             //��� mEventMap Ϊ�գ��� mMaxNumSockets ����Ϊ 0����ʾû���κ��¼�����
    else
        mMaxNumSockets = mEventMap.rbegin()->first + 1; //��� mEventMap ��Ϊ�գ����ȡ�¼�ӳ��������һ��Ԫ�أ��������ļ������������� 1���Ա�֤ mMaxNumSockets ��ʾ���ǿ��õ��ļ�������������
    //��������ļ�������+1��map���Զ��������һλ

    return true;
}

bool EPollPoller::removeIOEvent(IOEvent* event)             // �� epoll ���Ƴ�һ�� IOEvent
{
    int fd = event->getFd();
    if (fd < 0)
        return false;


    IOEventMap::iterator it = mEventMap.find(fd);               // ���¼�ӳ������Ƴ��¼�
    if (it != mEventMap.end())
        mEventMap.erase(it);

    if (mEventMap.empty())                                  // ��������ļ�������
        mMaxNumSockets = 0;
    else
        mMaxNumSockets = mEventMap.rbegin()->first + 1;

    return true;
}

void EPollPoller::handleEvent() {                                        // �����¼�

    epoll_event events[mMaxNumSockets];
    int ret;
    int rEvent;

    ret = epoll_wait(epollfd, events, mMaxNumSockets, -1);              // �ȴ�����������fd��Ӧ�¼��Ĳ������ɹ�������Ҫ������¼���Ŀ��ʧ�ܷ���0����ʾ�ȴ���ʱ��
    //epollfd:epollʵ���ľ��    events:epoll�ѷ������¼��ļ��ϴ��ں˸��Ƶ� events������          mMaxNumSockets:���ο��Է��ص�����¼���Ŀ   -1:û�м�⵽�¼�����ʱ���ȴ���ʱ�䣨������

    if (ret < 0) {                                                  // �������ֵС�� 0����������
        return;
    }
    else {
    }

    for (int i = 0; i < ret; i++)                                       // �Ѽ�������io�¼�ȫ���浽mIOEvent����
    {
        int sockfd = events[i].data.fd;
        mEventMap[sockfd]->setREvent(IOEvent::EVENT_READ);              // �����¼�����Ϊ���¼�
        mIOEvents.push_back(mEventMap[sockfd]);                         // ���¼���ӵ��¼��б���
    }

    for (auto& ioEvent : mIOEvents) {                               // �����¼��б��е�ÿ���¼�
        ioEvent->handleEvent();                                 //���˴����д���readCallback
    }

    mIOEvents.clear();                                          // ����¼��б�

}