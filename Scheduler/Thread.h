#pragma once
#ifndef TRLIB_THREAD_H
#define TRLIB_THREAD_H
#include<thread>

class Thread {
public:
	virtual ~Thread();

	bool start(void* arg);
	bool detach();
	bool join();

protected:
	Thread();
	virtual void run(void* arg) = 0;	//���麯��������������д

private:
	static void* threadRun(void*);

private:
	void* mArg;	//�̲߳���
	bool mIsStart;	//�߳��Ƿ�����
	bool mIsDetach;	//�߳��Ƿ����
	std::thread mThread;	//�̶߳���
};


#endif

