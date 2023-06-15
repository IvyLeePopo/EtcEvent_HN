#pragma once
#include "ThreadBase.h"


class CThreadNotifyCaller : public CThreadBase
{
	DECLARE_DYNCREATE(CThreadNotifyCaller)

public:
	CThreadNotifyCaller();           // protected constructor used by dynamic creation
	virtual ~CThreadNotifyCaller();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
//���غ���
protected:
	//��������������Դ
	void fnReleaseResource();
	
protected:
	DECLARE_MESSAGE_MAP()

	/*******************************�ڲ�������Ϣ����������*******************************/
	//�ڲ�������Ϣ���������� -- �ص�����֪ͨ���������
	afx_msg void OnMsgNotifyCaller(WPARAM wParam, LPARAM lParam);

};