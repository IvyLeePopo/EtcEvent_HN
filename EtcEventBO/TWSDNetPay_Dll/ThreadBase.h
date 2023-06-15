/*
 * Copyright(C) 2015,SQUIRREL ���˹����� ��������Ȩ����( All rights reserved. )
 * 
 * �ļ����ƣ�ThreadBase.h
 * ժ    Ҫ�����������ڲ��߳���ͳһ�̳нӿڣ������ļ�
 * ��ǰ�汾��1.0.0.1
 * ��    �ߣ�ROCY
 * �������ڣ�2015��11��11��
 */
#pragma once

#include "BaseDefine.h"

// CThreadBase

class CThreadBase : public CWinThread
{
	DECLARE_DYNCREATE(CThreadBase)

protected:
	CThreadBase();           // protected constructor used by dynamic creation
	virtual ~CThreadBase();
//ϵͳ���غ���
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

//�ӿڶ���
public:
	//���ݳ����������ò���
	void	SetParam(IN const tagTranstParam* pTranstParam,IN const HWND& hMainWnd); 
	//���ñ��������ļ�����·��
	void	SetLocalConfigFilePath(IN const CString& strFilePath){m_strPathLocalConfigFile = strFilePath;}
	//�Ƿ���������UI������Ϣ
	const BOOL IsAllowSendMsgToUI(IN const HWND& hWnd = NULL);
	//��������
	inline void Lock(void)  {::EnterCriticalSection(&m_cs);}
	//��������
	inline void UnLock(void){::LeaveCriticalSection(&m_cs);}
protected:
	DECLARE_MESSAGE_MAP()
//�麯��,����̳�
protected:
	//��������������Դ
	virtual void fnReleaseResource(){}
	//��ȡ������ص�������Ϣ(��ָ�������ļ�)
	virtual void fnReadSpecailConfig(){}
//Attributes
public:
	//�߳��˳����
	BOOL	m_bExit;
protected:
	//��������
	HWND	m_hMainWnd;
	//�߳��Ƿ������ڹ���״̬
	BOOL	m_bWorking;
	//����һ�δ�����ʾ
	CString	m_strLastErrorDesc;
	//�����ļ�����·��
	CString m_strPathLocalConfigFile;
	//��������������Ϣ
	tagTranstParam	m_sTranstParam;
	//�ٽ�����������
	CRITICAL_SECTION m_cs;
};


class CScopeLock
{
public:
    CScopeLock(CRITICAL_SECTION* lck)
        : mMutex(lck)
    {
		if(mMutex!=NULL)
		{
			::EnterCriticalSection(mMutex);
		}
        
    }
    ~CScopeLock()
    {
		if(mMutex!=NULL)
		{
			 ::LeaveCriticalSection(mMutex);
		}
       
    }
private:
    CRITICAL_SECTION* mMutex;
private:
    CScopeLock(const CScopeLock&);
    CScopeLock& operator = ( const CScopeLock&);

};


