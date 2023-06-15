#include "stdafx.h"
#include "MediaManage.h"
#include "SystemDiary.h"
#include "JsonHelper.h"

CMediaManage::CMediaManage()
{
    InitializeCriticalSection(&m_AccessMutex);
    m_MediaTask = 0;
}
CMediaManage::~CMediaManage()
{
    DeleteCriticalSection(&m_AccessMutex);
}

int CMediaManage::Destroy()
{
	trawe::CScopeLock tmp_lock(&m_AccessMutex);
	delete m_MediaTask;
	m_MediaTask=0;
	TW_TRACE_DIARY("媒体网关处理单元，删除成功");
	return 0;
}

int CMediaManage::Init()
{
    m_IsEnable = true;
    if(!m_IsEnable)
    {
        return -1;
    }
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_MediaTask != 0)
    {
        TW_TRACE_DIARY("媒体网关处理单元已经存在");
        return -1;
    }
	m_MediaTask = new CMediaUnit();
    if(m_MediaTask==0)
    {
        TW_TRACE_DIARY("媒体网关处理单元创建失败");
        return -1;
    }
    TW_TRACE_DIARY("媒体网关处理单元创建成功");
    m_MediaTask->Resume();
    return 0;
}


int CMediaManage::EventDealStart()
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_MediaTask == 0)
    {
        return -1;
    }
    return m_MediaTask->EventDealStart();
}


int CMediaManage::EventDealStop()
{
    trawe::CScopeLock tmp_lock(&m_AccessMutex);
    if(m_MediaTask == 0)
    {
        return -1;
    }
    return m_MediaTask->EventDealStop();
}





