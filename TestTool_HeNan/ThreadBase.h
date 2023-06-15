#pragma once

// CThreadBase

class CThreadBase : public CWinThread
{
	DECLARE_DYNCREATE(CThreadBase)

protected:
	CThreadBase();           // protected constructor used by dynamic creation
	virtual ~CThreadBase();
//系统重载函数
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

//接口定义
public:

	//锁定操作
	inline void Lock(void)  {::EnterCriticalSection(&m_cs);}
	//解锁操作
	inline void UnLock(void){::LeaveCriticalSection(&m_cs);}
protected:
	DECLARE_MESSAGE_MAP()
//虚函数,必须继承
protected:
	//清除本类申请的资源
	virtual void fnReleaseResource(){}
	//读取本类相关的配置信息(从指定配置文件)
	virtual void fnReadSpecailConfig(){}
//Attributes
public:
	//线程退出标记
	BOOL	m_bExit;
protected:
	//主窗体句柄
	HWND	m_hMainWnd;
	//线程句柄
	UINT	m_nMainThreadID;
	//线程是否正处于工作状态
	BOOL	m_bWorking;
	//临界区互锁变量
	CRITICAL_SECTION m_cs;
};


