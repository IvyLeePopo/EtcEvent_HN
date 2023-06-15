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
//重载函数
protected:
	//清除本类申请的资源
	void fnReleaseResource();
	
protected:
	DECLARE_MESSAGE_MAP()

	/*******************************内部被动消息触发处理函数*******************************/
	//内部被动消息触发处理函数 -- 回调函数通知组件调用者
	afx_msg void OnMsgNotifyCaller(WPARAM wParam, LPARAM lParam);

};