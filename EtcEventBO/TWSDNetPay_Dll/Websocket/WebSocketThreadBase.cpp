#include "StdAfx.h"
#include "WebSocketThreadBase.h"
#include <windows.h>

WebSocketThreadBase::WebSocketThreadBase(void)
{
	m_bCanRun = false;
//#ifdef WINVER
	InitializeCriticalSection(&m_criSection);
//#endif
}

WebSocketThreadBase::~WebSocketThreadBase(void)
{
}

void WebSocketThreadBase::vStart()
{
	AfxBeginThread(_threadFunc, (void*)this);
	m_bCanRun = true;
}

void WebSocketThreadBase::vStop()
{
	m_bCanRun = false;
}

bool WebSocketThreadBase::isCanRun()
{
	return m_bCanRun;
}

int WebSocketThreadBase::circle()
{
	return TRUE;
}

void WebSocketThreadBase::_lock()
{
//#ifdef WINVER
	EnterCriticalSection(&m_criSection);
//#endif
}

void WebSocketThreadBase::_unLock()
{
//#ifdef WINVER
	LeaveCriticalSection(&m_criSection);
//#endif
}

UINT WebSocketThreadBase::_threadFunc(void* pArgs)
{
	if(NULL == pArgs)
		return FALSE;

	WebSocketThreadBase* pThreadBase = (WebSocketThreadBase*)pArgs;
	pThreadBase->circle();

	return TRUE;
}
