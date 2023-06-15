#include "stdafx.h"
#include "ThreadReceiveRespMsg.h"

#include "TestDevIFDlg.h"

// CThreadSMManager

extern CTestDevIFDlg *g_DevIFDlg;

IMPLEMENT_DYNCREATE(CThreadReceiveRespMsg, CThreadBase)

CThreadReceiveRespMsg::CThreadReceiveRespMsg()
{

}

CThreadReceiveRespMsg::~CThreadReceiveRespMsg()
{

}

BOOL CThreadReceiveRespMsg::InitInstance()
{
	g_DevIFDlg->ReceiveMsgThreadID = m_nThreadID;
	char log[128] = {0};
	_snprintf_s(log, 128, "[应答消息接收线程]：线程开始启动,线程ID: %ld", m_nThreadID);
	g_DevIFDlg->log(log);

	return TRUE;
}

int CThreadReceiveRespMsg::ExitInstance()
{
	fnReleaseResource();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadReceiveRespMsg, CThreadBase)
	ON_THREAD_MESSAGE(WM_THREAD_ETCEVENT_MESSAGE_SHOWLOG, OnMsgThreadShowEtcEventLog)	
	ON_THREAD_MESSAGE(WM_THREAD_NETPAY_MESSAGE_SHOWLOG, OnMsgThreadShowNetPayLog)
END_MESSAGE_MAP()


void CThreadReceiveRespMsg::fnReleaseResource()
{
	return;
}

void CThreadReceiveRespMsg::OnMsgThreadShowEtcEventLog(WPARAM wParam, LPARAM lParam)
{
	g_DevIFDlg->OnHWNDMsgShowEtcEventLog(wParam, lParam);
	return;
}


void CThreadReceiveRespMsg::OnMsgThreadShowNetPayLog(WPARAM wParam, LPARAM lParam)
{
	g_DevIFDlg->OnHWNDMsgShowNetPayResultLog(wParam, lParam);
	return;
}


