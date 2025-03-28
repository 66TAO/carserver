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
    std::unique_lock<std::mutex> lck(mMtx); //加锁
    mTaskQue.push(task);    //任务入队
    mCon.notify_one();  //唤醒一个线程
}

void ThreadPool::loop() {
    while (!mQuit) {
        std::unique_lock<std::mutex> lck(mMtx); //加锁
        if (mTaskQue.empty()) {
            mCon.wait(lck); //等待条件变量
        }
        if (mTaskQue.empty()) {
            return;
        }
        Task* task = mTaskQue.front();  //取出任务
        mTaskQue.pop();
        lck.unlock();   //解锁
        task->handle(); //处理任务
    }
}

void ThreadPool::createThreads() {
    std::unique_lock<std::mutex> lck(mMtx); // 创建lck锁定互斥量mMtx。保证线程启动过程的线程安全，避免竞态条件和同步问题。
    for (auto& mThread : mThreads) {    //范围for循环，遍历mThreads容器的每个线程。采用引用赋值，避免拷贝
        mThread.start(this);
        // this指针作为参数传递，意味着每个线程都会得到一个指向创建它们的ThreadPool实例的引用。
        // 这允许线程与ThreadPool实例交互
    }
}

void ThreadPool::cancelThreads() {
    std::unique_lock<std::mutex> lck(mMtx);
    mQuit = true;
    mCon.notify_all();  //唤醒所有线程，防止有线程在等待条件变量
    for (auto& mThread : mThreads) {
        mThread.join(); //等待线程结束
    }
    mThreads.clear();   //清空线程数组
}

void ThreadPool::MThread::run(void* arg) {
    ThreadPool* threadPool = (ThreadPool*)arg;  //arg参数是指向ThreadPool对象的指针
    threadPool->loop();
}
