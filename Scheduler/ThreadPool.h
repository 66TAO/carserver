#ifndef TRLIB_THREADPOOL_H
#define TRLIB_THREADPOOL_H
#include<queue>
#include<vector>
#include"Thread.h"
#include<mutex>
#include<condition_variable>

class Task {
public:
	typedef void (*TaskCallback)(void*);	//����ص�����������ָ�롣������һ����ΪTaskCallback�����ͱ���
	// �ú���ָ�������ص㣺��������void��һ����ΪTaskCallback�����ͱ�����
	// *TaskCallback��ʾ����һ�� ָ������ָ�� ��
	/* �������¼�������̻��̳߳�ʵ���У�����ִ����ɺ����ûص���������������֪ͨ״̬��*/
	static Task* createNew();
	Task();
	void setTaskCallback(TaskCallback cb, void* arg);
	void handle();

private:
	TaskCallback mTaskCallback;	//����ص�����
	void* mArg;	//�ص���������
};

class ThreadPool {
public:
	static ThreadPool* createNew(int num);
	explicit ThreadPool(int num);	//explicit�ؼ��֣���ֹ��ʽת��
	~ThreadPool();

	void addTask(Task* task, std::string name);

private:
	void loop();
	void createThreads();	//�����߳�
	void cancelThreads();	//ȡ���߳�
	class MThread : public Thread {
		void run(void* arg);
	};

private:
	std::queue<Task*>mTaskQue;	//������У�ÿ��������һ��Task���󣬹�����Դ
	std::mutex mMtx;	//����������֤ÿ��ֻ��һ���߳��ܷ����������
	std::condition_variable mCon;	//���������������߳�ͬ��
	std::vector<MThread> mThreads;	//�߳����飬������Դ
	bool mQuit;	//�̳߳��˳���־��������Դ
};
/*���������Ϊ��ʱ�������̻߳�ȴ�����������mCon�ϣ�
�����µ�������ӵ��������ʱ����������mCon�ᱻ���ѣ������̻߳����ִ�С�
���������Ҫ������mMtx�ı������Է�ֹ��̬������
���ʹ�����Դ�������*/

#endif // !TRLIB_THREADPOOL_H

