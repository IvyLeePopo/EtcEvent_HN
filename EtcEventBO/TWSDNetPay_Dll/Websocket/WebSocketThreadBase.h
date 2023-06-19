#pragma once
#include "singleton.h"

class WebSocketThreadBase
{
public:
	WebSocketThreadBase(void);
	virtual ~WebSocketThreadBase(void);

public:
	// 开启线程
	void vStart();

	// 结束线程
	virtual void vStop();

	// 线程是否可运行
	virtual bool isCanRun();

	// 子类重写此函数用于接收线程逻辑
	virtual int circle();

	// 入锁、解锁临界区，不同的编译环境修改成其他的同步条件即可
	void _lock();
	void _unLock();
private:
	// 线程函数
	static UINT _threadFunc(void* pArgs);
	
protected:
	bool m_bCanRun;						// 是否可以运行

//#ifdef WINVER
	CRITICAL_SECTION m_criSection;		// 临界区，定义一个，需要数据同步的自取即可
//#endif
};
