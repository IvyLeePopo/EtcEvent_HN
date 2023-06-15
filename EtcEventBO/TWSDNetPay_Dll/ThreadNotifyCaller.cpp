#include "stdafx.h"
#include "ThreadNotifyCaller.h"
#include "TWSDNetPay_Dll.h"
#include "BaseDefine.h"

extern CTWSDNetPay_DllApp theApp;

// CThreadSMManager

IMPLEMENT_DYNCREATE(CThreadNotifyCaller, CThreadBase)

CThreadNotifyCaller::CThreadNotifyCaller()
{

}

CThreadNotifyCaller::~CThreadNotifyCaller()
{

}

BOOL CThreadNotifyCaller::InitInstance()
{
	CTWSDNetPay_DllApp::NotifyCallerThreadID = m_nThreadID;
	char log[128] = {0};
	_snprintf(log, 128, "[EtcEvent应答通知线程]：线程开始启动,线程ID: %ld", CTWSDNetPay_DllApp::NotifyCallerThreadID);
	theApp.RecordLog(log);
	return TRUE;
}

int CThreadNotifyCaller::ExitInstance()
{
	fnReleaseResource();
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadNotifyCaller, CThreadBase)
	ON_THREAD_MESSAGE(WM_THREAD_DEVICE_ETC_EVENT_RESP, OnMsgNotifyCaller)	
END_MESSAGE_MAP()


void CThreadNotifyCaller::fnReleaseResource()
{
	return;
}

void CThreadNotifyCaller::OnMsgNotifyCaller(WPARAM wParam, LPARAM lParam)
{
	theApp.RecordLog("[EtcEvent应答通知线程]：开始调用回调函数通知车道软件特情处理结果......");
	unsigned short EtcEventId = (unsigned short)wParam;
	/*
	if (NULL != CTWSDNetPay_DllApp::m_CallbackNotifyEtcEvent)
    {
        CTWSDNetPay_DllApp::m_CallbackNotifyEtcEvent(6, EtcEventId, CTWSDNetPay_DllApp::m_pIndPtrEtcEvent);
		theApp.RecordLog(_T("[组件回调通知线程]：完成调用回调函数通知车道软件特情处理结果"));
    }
    else
    {
        theApp.RecordLog(_T("[组件回调通知线程]：通知外部调用者的回调函数指针为空，无法通知调用者"));
    }*/

	if (EtcEventId == 630) //设备序列号
	{
		char log[128] = {0};
		theApp.m_strTWProductSerialNo.Format("%s",(char*)theApp.m_pIndPtrEtcEvent);
		_snprintf(log, 128, "[EtcEvent应答通知线程]：线程ID: %ld,设备序列号： %s", CTWSDNetPay_DllApp::EtcEventCallerThreadID,(LPCTSTR)theApp.m_strTWProductSerialNo);
		theApp.RecordLog(log);
		return;
	}

	if (0 != CTWSDNetPay_DllApp::EtcEventCallerThreadID)
    {
        ::PostThreadMessage(CTWSDNetPay_DllApp::EtcEventCallerThreadID, CTWSDNetPay_DllApp::EtcEventRespMessgeID, (WPARAM)MAKELONG(6, EtcEventId), (LPARAM)0);
		char log[128] = {0};
		_snprintf(log, 128, "[EtcEvent应答通知线程]：线程ID: %ld", CTWSDNetPay_DllApp::EtcEventCallerThreadID);
		theApp.RecordLog(log);
    }

    if (::IsWindow(CTWSDNetPay_DllApp::EtcEventCallerWnd))
    {
        ::PostMessage(CTWSDNetPay_DllApp::EtcEventCallerWnd, CTWSDNetPay_DllApp::EtcEventRespMessgeID, (WPARAM)MAKELONG(6, EtcEventId), (LPARAM)0);
    }

	return;
}


