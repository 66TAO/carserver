#include "ThreadPool.h"
#include"iostream"


Task* Task::createNew()
{
    return new Task();
}

Task::Task() :
    mTaskCallback(NULL),
    mArg(NULL)
{

}

void Task::setTaskCallback(TaskCallback cb, void* arg)
{
    mTaskCallback = cb;
    mArg = arg;
}

void Task::handle()
{
    if (mTaskCallback) {
        mTaskCallback(mArg);
    }
}

ThreadPool* ThreadPool::createNew(int num)
{
    return new ThreadPool(num);
}

ThreadPool::ThreadPool(int num) :
    mThreads(num),
    mQuit(false)
{
    createThreads();
}

ThreadPool::~ThreadPool() {
    cancelThreads();
}

void ThreadPool::addTask(Task* task, std::string name) {
    std::cout << "addTask " << name << std::endl;
    std::unique_lock<std::mutex> lck(mMtx); //����
    mTaskQue.push(task);    //�������
    mCon.notify_one();  //����һ���߳�
}

void ThreadPool::loop() {
    while (!mQuit) {
        std::unique_lock<std::mutex> lck(mMtx); //����
        if (mTaskQue.empty()) {
            mCon.wait(lck); //�ȴ���������
        }
        if (mTaskQue.empty()) {
            return;
        }
        Task* task = mTaskQue.front();  //ȡ������
        mTaskQue.pop();
        lck.unlock();   //����
        task->handle(); //��������
    }
}

void ThreadPool::createThreads() {
    std::unique_lock<std::mutex> lck(mMtx); // ����lck����������mMtx����֤�߳��������̵��̰߳�ȫ�����⾺̬������ͬ�����⡣
    for (auto& mThread : mThreads) {    //��Χforѭ��������mThreads������ÿ���̡߳��������ø�ֵ�����⿽��
        mThread.start(this);
        // thisָ����Ϊ�������ݣ���ζ��ÿ���̶߳���õ�һ��ָ�򴴽����ǵ�ThreadPoolʵ�������á�
        // �������߳���ThreadPoolʵ������
    }
}

void ThreadPool::cancelThreads() {
    std::unique_lock<std::mutex> lck(mMtx);
    mQuit = true;
    mCon.notify_all();  //���������̣߳���ֹ���߳��ڵȴ���������
    for (auto& mThread : mThreads) {
        mThread.join(); //�ȴ��߳̽���
    }
    mThreads.clear();   //����߳�����
}

void ThreadPool::MThread::run(void* arg) {
    ThreadPool* threadPool = (ThreadPool*)arg;  //arg������ָ��ThreadPool�����ָ��
    threadPool->loop();
}
