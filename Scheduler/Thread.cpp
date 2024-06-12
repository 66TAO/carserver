#include"Thread.h"

Thread::Thread() :
	mArg(NULL),
	mIsStart(false),
	mIsDetach(false)
{

}

Thread::~Thread() {
	if (mIsStart && !mIsDetach) {
		detach();
	}
}

bool Thread::start(void* arg) {
	mArg = arg;
	mThread = std::thread(&threadRun, this);	//�������̣߳�ִ��threadRun������������this��ָ��ǰ����
	mIsStart = true;
	return true;
}

bool Thread::detach() {
	if (!mIsStart) {
		return false;
	}
	if (mIsDetach) {
		return true;
	}
	mThread.detach();	//�����̣߳�std::thread���е�detach����
	mIsDetach = true;
	return true;
}

bool Thread::join() {
	if (!mIsStart || mIsDetach) {
		return false;
	}
	mThread.join();	//�ȴ��߳̽�����std::thread���е�join����
	return true;
}

void* Thread::threadRun(void* arg) {	//arg������ָ��Thread�����ָ��
	Thread* thread = (Thread*)arg;
	thread->run(thread->mArg);	//ִ��run������runΪ���飬����������д����
	return NULL;
}
