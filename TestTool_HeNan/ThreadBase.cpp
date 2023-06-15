/*
 * Copyright(C) 北京特微智能 保留所有权利。( All rights reserved. )
 * 
 * 文件名称：ThreadBase.cpp
 * 摘    要：车道软件内部线程类统一继承接口，实现文件
 * 当前版本：1.0.0.0
 * 作    者：朱道明
 * 创建日期：2020年10月18日
 */

#include "stdafx.h"
#include "ThreadBase.h"


// CThreadBase

IMPLEMENT_DYNCREATE(CThreadBase, CWinThread)

CThreadBase::CThreadBase()
	:m_bExit(FALSE)
	,m_hMainWnd(NULL)
	,m_nMainThreadID(0)
	,m_bWorking(FALSE)
{

}

CThreadBase::~CThreadBase()
{
}

BOOL CThreadBase::InitInstance()
{
	//初始化互锁变量
//	::InitializeCriticalSection(&m_cs);
	return TRUE;
}

int CThreadBase::ExitInstance()
{
	//清除互锁变量
//	::DeleteCriticalSection(&m_cs);
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadBase, CWinThread)
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////////////////////
// 函数名称：CThreadBase::OnIdle
// 功能描述：基本线程类空闲处理函数
// 输入参数：LONG lCount
// 输出参数：BOOL
// 编写人员：朱道明
// 编写时间：2020年10月18日
// 修改人员：
// 修改时间：
// 函数版本：1.0.0.0
// 备注说明：
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CThreadBase::OnIdle(LONG lCount)
{
	if(m_bExit)
	{
		::PostQuitMessage(0);
		return FALSE;
	}
	Sleep(1);

	return TRUE;
}
