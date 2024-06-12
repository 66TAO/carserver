#include "EventScheduler.h"
#include "SocketsOps.h"
#include "SelectPoller.h"
//#include "PollPoller.h"
#include "EPollPoller.h"
#include "../Base/Log.h"

#include <sys/eventfd.h>

EventScheduler* EventScheduler::createNew(PollerType type)                      //����type��ʾ��ѯ�������ͣ�������POLLER_SELECT��POLLER_POLL��POLLER_EPOLL
{
    if (type != POLLER_SELECT && type != POLLER_POLL && type != POLLER_EPOLL)               // �����ѯ�������Ƿ�Ϸ�
        return NULL;

    return new EventScheduler(type);                                    // ����������һ���µ�EventScheduler����
}

EventScheduler::EventScheduler(PollerType type) : mQuit(false) {


    switch (type) {                                         // ������ѯ������ѡ����ʵ���ѯ��

    case POLLER_SELECT:
        mPoller = SelectPoller::createNew();
        break;

        //case POLLER_POLL:
        //    mPoller = PollPoller::createNew();
        //    break;

    case POLLER_EPOLL:
        mPoller = EPollPoller::createNew();
        break;

    default:                                            // �����ѯ�����Ͳ��Ϸ�����ֱ���˳�����
        _exit(-1);
        break;
    }

}

EventScheduler::~EventScheduler()                       // EventScheduler��������������ͷ���ѯ��������ڴ�
{

    delete mPoller;

}

bool EventScheduler::addTriggerEvent(TriggerEvent* event)               // ���¼����������һ���������¼�
{
    mTriggerEvents.push_back(event);                                // ���¼���ӵ��������¼��б���

    return true;
}

bool EventScheduler::addIOEvent(IOEvent* event)                         // ���¼����������һ��IO�¼�
{
    return mPoller->addIOEvent(event);
}

bool EventScheduler::updateIOEvent(IOEvent* event)                      // �����¼��������е�һ��IO�¼�
{
    return mPoller->updateIOEvent(event);
}

bool EventScheduler::removeIOEvent(IOEvent* event)                      // ���¼����������Ƴ�һ��IO�¼�
{
    return mPoller->removeIOEvent(event);
}

void EventScheduler::loop() {                                       // �¼�����������ѭ�������������¼���������ѯ������IO�¼�
    while (!mQuit) {
        handleTriggerEvents();                                      //����TriggerEvents
        mPoller->handleEvent();                                     //����IOEvents    //epoll->handleEvent
    }
}

void EventScheduler::handleTriggerEvents()                                               // ���������¼��ķ���
{
    if (!mTriggerEvents.empty())                                                        // ����������¼��б�Ϊ��
    {
        for (std::vector<TriggerEvent*>::iterator it = mTriggerEvents.begin();          // �����������¼��б����δ���ÿ���¼�
            it != mTriggerEvents.end(); ++it)
        {
            (*it)->handleEvent();                                           //TriggerEvent->hendleEvent
        }

        mTriggerEvents.clear();                                                             // ��մ������¼��б�
    }
}

Poller* EventScheduler::poller() {                                                  // �����¼���������ʹ�õ���ѯ������
    return mPoller;
}