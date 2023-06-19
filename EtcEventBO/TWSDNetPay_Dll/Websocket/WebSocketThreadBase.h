#pragma once
#include "singleton.h"

class WebSocketThreadBase
{
public:
	WebSocketThreadBase(void);
	virtual ~WebSocketThreadBase(void);

public:
	// �����߳�
	void vStart();

	// �����߳�
	virtual void vStop();

	// �߳��Ƿ������
	virtual bool isCanRun();

	// ������д�˺������ڽ����߳��߼�
	virtual int circle();

	// �����������ٽ�������ͬ�ı��뻷���޸ĳ�������ͬ����������
	void _lock();
	void _unLock();
private:
	// �̺߳���
	static UINT _threadFunc(void* pArgs);
	
protected:
	bool m_bCanRun;						// �Ƿ��������

//#ifdef WINVER
	CRITICAL_SECTION m_criSection;		// �ٽ���������һ������Ҫ����ͬ������ȡ����
//#endif
};
